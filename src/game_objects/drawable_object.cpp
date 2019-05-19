#include "drawable_object.h"
#include "../graphics/buffers/uniform_buffer.h"

DrawableObject::DrawableObject(Game* game, Material* material, Material* shadowMaterial)
    : GameObject(game), material(material), shadowMaterial(shadowMaterial) {

}

DrawableObject::~DrawableObject() {
    for (auto buffer : uniformBuffers) {
        delete buffer;
    }
    delete offscreenUniformBuffer;

    delete indexBuffer;
    delete vertexBuffer;
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
    if (vkAllocateDescriptorSets(game->getGraphics()->getLogicalDevice()->getDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < swapChainImageSize; i++) {
        VkDescriptorBufferInfo bufferInfo = {};
        bufferInfo.buffer = uniformBuffers[i]->getBuffer();
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        material->createDescriptorSet(bufferInfo, descriptorSets[i]);
    }

    std::vector<VkDescriptorSetLayout> offscreenLayouts(swapChainImageSize, shadowMaterial->descriptorSetLayout);
    VkDescriptorSetAllocateInfo offscreenAllocInfo = {};
    offscreenAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    offscreenAllocInfo.descriptorPool = game->getGraphics()->getRenderer()->getDescriptorPool();
    offscreenAllocInfo.descriptorSetCount = 1;
    offscreenAllocInfo.pSetLayouts = offscreenLayouts.data();

    if (vkAllocateDescriptorSets(game->getGraphics()->getLogicalDevice()->getDevice(), &offscreenAllocInfo, &offscreenDescriptorSets) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    VkDescriptorBufferInfo bufferInfo = {};
    bufferInfo.buffer = offscreenUniformBuffer->getBuffer();
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(UniformBufferObjectOffscreen);

    shadowMaterial->createDescriptorSet(bufferInfo, offscreenDescriptorSets);
}

void DrawableObject::updateUniformBuffer(uint32_t currentImage, glm::mat4 view, glm::mat4 projection, glm::vec3 lightPos) {
    //    position.x = glm::clamp(position.x + (rand() % 2 - .5f) / 500, -3.0f, 3.0f);
    //    position.y = glm::clamp(position.y + (rand() % 2 - .5f) / 500, -3.0f, 3.0f);
    //    position.z = glm::clamp(position.z + (rand() % 2 - .5f) / 500, -3.0f, 3.0f);

    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo = {};
    ubo.model = glm::mat4(1.0f);
    ubo.model = glm::translate(ubo.model, position);//position
    ubo.model = glm::rotate(ubo.model, rotation.y, glm::vec3(0.0f, 0.0f, 1.0f));//rotation
    ubo.model = glm::scale(ubo.model, scale);

    ubo.view = view;
    ubo.projection = projection;
    ubo.projection[1][1] *= -1;

    //TODO: change when rendered from light
    ubo.lightPos = lightPos;
    ubo.depthBiasMVP = ubo.view * ubo.projection;

    auto device = game->getGraphics()->getLogicalDevice()->getDevice();
    void* data;
    vkMapMemory(device, uniformBuffers[currentImage]->getMemory(), 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(device, uniformBuffers[currentImage]->getMemory());

    UniformBufferObjectOffscreen uboOffscreen = {};
    uboOffscreen.model = ubo.model;
    uboOffscreen.depthVP = ubo.view * ubo.projection;

    void* offscreenData;
    vkMapMemory(device, offscreenUniformBuffer->getMemory(), 0, sizeof(uboOffscreen), 0, &offscreenData);
    memcpy(offscreenData, &uboOffscreen, sizeof(uboOffscreen));
    vkUnmapMemory(device, offscreenUniformBuffer->getMemory());
}

void DrawableObject::createUniformBuffers(size_t swapChainImageSize) {
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);
    uniformBuffers.resize(swapChainImageSize);

    auto device = game->getGraphics()->getLogicalDevice();
    for (size_t i = 0; i < swapChainImageSize; i++) {
        uniformBuffers[i] = new UniformBuffer(device, bufferSize);
    }

    VkDeviceSize offscreenBufferSize = sizeof(UniformBufferObjectOffscreen);
    offscreenUniformBuffer = new UniformBuffer(device, offscreenBufferSize);
}

std::vector<Vertex> DrawableObject::getVertices() {
    return vertices;
}

std::vector<uint32_t> DrawableObject::getIndices() {
    return indices;
}