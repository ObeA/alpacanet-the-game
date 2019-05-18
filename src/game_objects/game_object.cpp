#include "game_object.h"

GameObject::GameObject(Game* game, Material* material, Material* shadowMaterial)
    : game(game), material(material), shadowMaterial(shadowMaterial) {

}

GameObject::~GameObject() {
    for (auto buffer : uniformBuffers) {
        delete buffer;
    }
    delete offscreenUniformBuffer;

    delete indexBuffer;
    delete vertexBuffer;
}

void GameObject::start() {
    createVertexBuffer();
    createIndexBuffer();

    auto numberOfSwapchainImages = game->getGraphics()->getRenderer()->getSwapchain()->getImages().size();
    createUniformBuffers(numberOfSwapchainImages);
    createDescriptorSet(numberOfSwapchainImages);
}

void GameObject::draw(VkCommandBuffer cmdbuffer, size_t bufferOffset) {
	VkBuffer vertexBuffers[] = { vertexBuffer->getBuffer() };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(cmdbuffer, 0, 1, vertexBuffers, offsets);
	vkCmdBindIndexBuffer(cmdbuffer, indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);

	vkCmdDrawIndexed(cmdbuffer, static_cast<uint32_t>(getIndices().size()), 1, 0, 0, 0);
}

void GameObject::createVertexBuffer() {
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

void GameObject::createIndexBuffer() {
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

void GameObject::createDescriptorSet(size_t swapChainImageSize) {
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

std::vector<Vertex> GameObject::getVertices() {
	return vertices;
}

std::vector<uint32_t> GameObject::getIndices() {
	return indices;
}