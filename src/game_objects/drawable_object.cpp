#include "drawable_object.h"
#include "../graphics/buffers/uniform_buffer.h"
#include "../graphics/vulkan_utilities.h"
#include "light.h"

DrawableObject::DrawableObject(Game* game, Material* material, Material* shadowMaterial)
    : GameObject(game), material(material), shadowMaterial(shadowMaterial) {

}

DrawableObject::~DrawableObject() {
    for (auto buffer : uniformBuffers) {
        delete buffer;
    }

    for (auto buffer : offscreenUniformBuffers) {
        delete buffer;
    }


    delete indexBuffer;
    delete vertexBuffer;

    vkFreeDescriptorSets(game->getGraphics()->getLogicalDevice()->getDevice(), game->getGraphics()->getRenderer()->getDescriptorPool(), descriptorSets.size(), descriptorSets.data());
    if (!offscreenDescriptorSets.empty()) {
        vkFreeDescriptorSets(game->getGraphics()->getLogicalDevice()->getDevice(), game->getGraphics()->getRenderer()->getDescriptorPool(), offscreenDescriptorSets.size(), offscreenDescriptorSets.data());
    }
}

void DrawableObject::start() {
    createVertexBuffer();
    createIndexBuffer();

    auto numberOfSwapchainImages = game->getGraphics()->getRenderer()->getSwapchain()->getImages().size();
    createUniformBuffers(numberOfSwapchainImages);
    createDescriptorSet(numberOfSwapchainImages);
}

void DrawableObject::draw(VkCommandBuffer cmdbuffer, size_t bufferOffset) {
    VkBuffer vertexBuffers[] = { vertexBuffer->getBuffer() };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(cmdbuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(cmdbuffer, indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);

    vkCmdDrawIndexed(cmdbuffer, static_cast<uint32_t>(getIndices().size()), 1, 0, 0, 0);
}

void DrawableObject::createVertexBuffer() {
    VkDeviceSize bufferSize = sizeof(getVertices()[0]) * getVertices().size();

    Buffer stagingBuffer(
        game->getGraphics()->getLogicalDevice(),
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        getVertices().data()
    );

    vertexBuffer = new Buffer(
        game->getGraphics()->getLogicalDevice(),
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );

    stagingBuffer.copyTo(vertexBuffer);
}

void DrawableObject::createIndexBuffer() {
    VkDeviceSize bufferSize = sizeof(getIndices()[0]) * getIndices().size();

    Buffer stagingBuffer(
        game->getGraphics()->getLogicalDevice(),
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        getIndices().data()
    );

    indexBuffer = new Buffer(
        game->getGraphics()->getLogicalDevice(),
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );

    stagingBuffer.copyTo(indexBuffer);
}

void DrawableObject::createDescriptorSet(size_t swapChainImageSize) {
    std::vector<VkDescriptorSetLayout> layouts(swapChainImageSize, material->descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = game->getGraphics()->getRenderer()->getDescriptorPool();
    allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImageSize);
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(swapChainImageSize);
    VK_CHECK_RESULT(vkAllocateDescriptorSets(game->getGraphics()->getLogicalDevice()->getDevice(), &allocInfo, descriptorSets.data()))

    for (size_t i = 0; i < swapChainImageSize; i++) {
        VkDescriptorBufferInfo bufferInfo = {};
        bufferInfo.buffer = uniformBuffers[i]->getBuffer();
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        material->createDescriptorSet(bufferInfo, descriptorSets[i]);
    }

    if (shadowMaterial == nullptr) {
        return;
    }

    std::vector<VkDescriptorSetLayout> offscreenLayouts(swapChainImageSize, shadowMaterial->descriptorSetLayout);
    VkDescriptorSetAllocateInfo offscreenAllocInfo = {};
    offscreenAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    offscreenAllocInfo.descriptorPool = game->getGraphics()->getRenderer()->getDescriptorPool();
    offscreenAllocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImageSize);
    offscreenAllocInfo.pSetLayouts = offscreenLayouts.data();

    offscreenDescriptorSets.resize(swapChainImageSize);
    VK_CHECK_RESULT(vkAllocateDescriptorSets(game->getGraphics()->getLogicalDevice()->getDevice(), &offscreenAllocInfo, offscreenDescriptorSets.data()))

    for (size_t i = 0; i < swapChainImageSize; i++) {
        VkDescriptorBufferInfo bufferInfo = {};
        bufferInfo.buffer = offscreenUniformBuffers[i]->getBuffer();
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObjectOffscreen);

        shadowMaterial->createDescriptorSet(bufferInfo, offscreenDescriptorSets[i]);
    }
}

void DrawableObject::updateUniformBuffer(uint32_t currentImage, Camera* camera, Light* light) {
    UniformBufferObject ubo = {};
    ubo.model = glm::mat4(1.0f);                                // identity
    ubo.model = glm::translate(ubo.model, position);            // position
    ubo.model = glm::rotate(ubo.model, rotation.z, camera->up); // rotation
    ubo.model = glm::scale(ubo.model, scale);                   // scale

    ubo.view = camera->getViewMatrix();
    ubo.projection = camera->getProjectionMatrix();
    ubo.projection[1][1] *= -1;

    // Offscreen rendering
    UniformBufferObjectOffscreen uboOffscreen = {};
    uboOffscreen.depthMVP = light->getProjectionMatrix() * light->getViewMatrix(camera->up) * ubo.model;
    // end offscreen rendering

    ubo.lightPos = light->position;
    ubo.lightSpace = uboOffscreen.depthMVP;

    auto device = game->getGraphics()->getLogicalDevice()->getDevice();
    void* data;
    vkMapMemory(device, uniformBuffers[currentImage]->getMemory(), 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(device, uniformBuffers[currentImage]->getMemory());

    if (shadowMaterial == nullptr) {
        return;
    }

    // Update offscreen ubo
    void* offscreenData;
    vkMapMemory(device, offscreenUniformBuffers[currentImage]->getMemory(), 0, sizeof(UniformBufferObjectOffscreen), 0, &offscreenData);
    memcpy(offscreenData, &uboOffscreen, sizeof(UniformBufferObjectOffscreen));
    vkUnmapMemory(device, offscreenUniformBuffers[currentImage]->getMemory());
}

void DrawableObject::createUniformBuffers(size_t swapChainImageSize) {
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);
    uniformBuffers.resize(swapChainImageSize);

    auto device = game->getGraphics()->getLogicalDevice();
    for (size_t i = 0; i < swapChainImageSize; i++) {
        uniformBuffers[i] = new UniformBuffer(device, bufferSize);
    }

    if (shadowMaterial == nullptr) {
        return;
    }

    VkDeviceSize offscreenBufferSize = sizeof(UniformBufferObjectOffscreen);
    offscreenUniformBuffers.resize(swapChainImageSize);
    for (size_t i = 0; i < swapChainImageSize; i++) {
        offscreenUniformBuffers[i] = new UniformBuffer(device, offscreenBufferSize);
    }
}

std::vector<Vertex> DrawableObject::getVertices() {
    return vertices;
}

std::vector<uint32_t> DrawableObject::getIndices() {
    return indices;
}

Bounds DrawableObject::getBounds() const {
    return bounds.getScaledCopy(scale);
}