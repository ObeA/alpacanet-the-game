//
// Created by Leon on 4/28/2019.
//

#include "Cube.h"

void Cube::generate(size_t swapchainImageSize) {
    createVertexBuffer();
    createIndexBuffer();
    createUniformBuffers(swapchainImageSize);
    createDescriptorSet(swapchainImageSize);
}

void Cube::updateUniformBuffer(uint32_t currentImage, glm::mat4 perspective) {
//    position.x = glm::clamp(position.x + (rand() % 2 - .5f) / 500, -3.0f, 3.0f);
//    position.y = glm::clamp(position.y + (rand() % 2 - .5f) / 500, -3.0f, 3.0f);
//    position.z = glm::clamp(position.z + (rand() % 2 - .5f) / 500, -3.0f, 3.0f);

    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo = {};
    ubo.model = glm::mat4(1.0f);
    ubo.model = glm::translate(ubo.model, position);//position
    ubo.model = glm::rotate(ubo.model, time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));//rotation

    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = perspective;
    ubo.proj[1][1] *= -1;

    void* data;
    vkMapMemory(window->device, uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(window->device, uniformBuffersMemory[currentImage]);
}

void Cube::cleanup(size_t swapchainImages) {
    for (size_t i = 0; i < swapchainImages; i++) {
        vkDestroyBuffer(window->device, uniformBuffers[i], nullptr);
        vkFreeMemory(window->device, uniformBuffersMemory[i], nullptr);
    }

    vkDestroyBuffer(window->device, indexBuffer, nullptr);
    vkFreeMemory(window->device, indexBufferMemory, nullptr);

    vkDestroyBuffer(window->device, vertexBuffer, nullptr);
    vkFreeMemory(window->device, vertexBufferMemory, nullptr);
}

void Cube::draw(VkCommandBuffer cmdbuffer, VkPipelineLayout pipelineLayout, size_t bufferOffset) {
    VkBuffer vertexBuffers[] = { vertexBuffer };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(cmdbuffer, 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(cmdbuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);

    vkCmdBindDescriptorSets(cmdbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[bufferOffset], 0, nullptr);

    vkCmdDrawIndexed(cmdbuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
}

void Cube::createVertexBuffer() {
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    window->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(window->device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(window->device, stagingBufferMemory);

    window->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

    window->copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

    vkDestroyBuffer(window->device, stagingBuffer, nullptr);
    vkFreeMemory(window->device, stagingBufferMemory, nullptr);
}

void Cube::createIndexBuffer() {
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    window->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(window->device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(window->device, stagingBufferMemory);

    window->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

    window->copyBuffer(stagingBuffer, indexBuffer, bufferSize);

    vkDestroyBuffer(window->device, stagingBuffer, nullptr);
    vkFreeMemory(window->device, stagingBufferMemory, nullptr);
}

void Cube::createUniformBuffers(size_t swapChainImageSize) {
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    uniformBuffers.resize(swapChainImageSize);
    uniformBuffersMemory.resize(swapChainImageSize);

    for (size_t i = 0; i < swapChainImageSize; i++) {
        window->createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
    }
}

void Cube::createDescriptorSet(size_t swapChainImageSize) {
    std::vector<VkDescriptorSetLayout> layouts(swapChainImageSize, window->descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = window->descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImageSize);
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(swapChainImageSize);
    if (vkAllocateDescriptorSets(window->device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < swapChainImageSize; i++) {
        VkDescriptorBufferInfo bufferInfo = {};
        bufferInfo.buffer = uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        VkDescriptorImageInfo imageInfo = {};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = window->textureImageView;
        imageInfo.sampler = window->textureSampler;

        std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = descriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;

        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = descriptorSets[i];
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(window->device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}