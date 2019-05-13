#include "game_object.h"

void GameObject::generate(size_t swapchainImageSize) {
	createVertexBuffer();
	createIndexBuffer();
	createUniformBuffers(swapchainImageSize);
	createDescriptorSet(swapchainImageSize);
}

void GameObject::draw(VkCommandBuffer cmdbuffer, size_t bufferOffset) {
	VkBuffer vertexBuffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(cmdbuffer, 0, 1, vertexBuffers, offsets);

	vkCmdBindIndexBuffer(cmdbuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);

	vkCmdDrawIndexed(cmdbuffer, static_cast<uint32_t>(getIndices().size()), 1, 0, 0, 0);
}

void GameObject::cleanup(size_t swapchainImages) {
    auto graphics = game->getGraphics();
    auto device = graphics->getLogicalDevice()->getDevice();

	for (size_t i = 0; i < swapchainImages; i++) {
		vkDestroyBuffer(device, uniformBuffers[i], nullptr);
		vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
	}

	vkDestroyBuffer(device, indexBuffer, nullptr);
	vkFreeMemory(device, indexBufferMemory, nullptr);

	vkDestroyBuffer(device, vertexBuffer, nullptr);
	vkFreeMemory(device, vertexBufferMemory, nullptr);
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

	std::vector<VkDescriptorSetLayout> offscreenLayouts(swapChainImageSize, shadowMaterial->descriptorSetLayout);
	VkDescriptorSetAllocateInfo offscreenAllocInfo = {};
	offscreenAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	offscreenAllocInfo.descriptorPool = window->descriptorPool;
	offscreenAllocInfo.descriptorSetCount = 1;
	offscreenAllocInfo.pSetLayouts = offscreenLayouts.data();

	if (vkAllocateDescriptorSets(window->device, &offscreenAllocInfo, &offscreenDescriptorSets) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = offscreenUniformBuffer;
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(UniformBufferObjectOffscreen);

	shadowMaterial->createDescriptorSet(bufferInfo, offscreenDescriptorSets);
}

std::vector<Vertex> GameObject::getVertices() {
	return vertices;
}

std::vector<uint32_t> GameObject::getIndices() {
	return indices;
}