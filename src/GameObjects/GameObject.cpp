//
// Created by Leon on 4/28/2019.
//

#include "GameObject.h"

#include "../Materials/Material.h"

void GameObject::generate(size_t swapchainImageSize) {
	createVertexBuffer();
	createIndexBuffer();
	createUniformBuffers(swapchainImageSize);
	createDescriptorSet(swapchainImageSize);
}

void GameObject::draw(VkCommandBuffer cmdbuffer, VkPipelineLayout pipelineLayout, size_t bufferOffset) {
	VkBuffer vertexBuffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(cmdbuffer, 0, 1, vertexBuffers, offsets);

	vkCmdBindIndexBuffer(cmdbuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);

	vkCmdBindDescriptorSets(cmdbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[bufferOffset], 0, nullptr);

	vkCmdDrawIndexed(cmdbuffer, static_cast<uint32_t>(getIndices().size()), 1, 0, 0, 0);
}

void GameObject::cleanup(size_t swapchainImages) {
	for (size_t i = 0; i < swapchainImages; i++) {
		vkDestroyBuffer(window->device, uniformBuffers[i], nullptr);
		vkFreeMemory(window->device, uniformBuffersMemory[i], nullptr);
	}

	vkDestroyBuffer(window->device, indexBuffer, nullptr);
	vkFreeMemory(window->device, indexBufferMemory, nullptr);

	vkDestroyBuffer(window->device, vertexBuffer, nullptr);
	vkFreeMemory(window->device, vertexBufferMemory, nullptr);
}

void GameObject::createVertexBuffer() {
	VkDeviceSize bufferSize = sizeof(getVertices()[0]) * getVertices().size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	window->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(window->device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, getVertices().data(), (size_t)bufferSize);
	vkUnmapMemory(window->device, stagingBufferMemory);

	window->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

	window->copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

	vkDestroyBuffer(window->device, stagingBuffer, nullptr);
	vkFreeMemory(window->device, stagingBufferMemory, nullptr);
}

void GameObject::createIndexBuffer() {
	VkDeviceSize bufferSize = sizeof(getIndices()[0]) * getIndices().size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	window->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(window->device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, getIndices().data(), (size_t)bufferSize);
	vkUnmapMemory(window->device, stagingBufferMemory);

	window->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

	window->copyBuffer(stagingBuffer, indexBuffer, bufferSize);

	vkDestroyBuffer(window->device, stagingBuffer, nullptr);
	vkFreeMemory(window->device, stagingBufferMemory, nullptr);
}

void GameObject::createDescriptorSet(size_t swapChainImageSize) {
	std::vector<VkDescriptorSetLayout> layouts(swapChainImageSize, material->descriptorSetLayout);
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

		material->createDescriptorSet(bufferInfo, descriptorSets[i]);
	}
}

std::vector<Vertex> GameObject::getVertices() {
	return vertices;
}

std::vector<uint16_t> GameObject::getIndices() {
	return indices;
}