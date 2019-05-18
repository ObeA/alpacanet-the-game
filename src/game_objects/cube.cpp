#include "cube.h"
#include "../materials/material.h"
#include "../graphics/buffers/uniform_buffer.h"

void Cube::updateUniformBuffer(uint32_t currentImage, glm::mat4 view, glm::mat4 projection, glm::vec3 lightPos) {
	position.x = glm::clamp(position.x + (rand() % 2 - .5f) / 500, -3.0f, 3.0f);
	position.y = glm::clamp(position.y + (rand() % 2 - .5f) / 500, -3.0f, 3.0f);
	position.z = glm::clamp(position.z + (rand() % 2 - .5f) / 500, -3.0f, 3.0f);

	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo = {};
	ubo.model = glm::mat4(1.0f);
	ubo.model = glm::translate(ubo.model, position);//position
	ubo.model = glm::rotate(ubo.model, time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));//rotation
	ubo.model = glm::scale(ubo.model, scale);

	ubo.view = view;
	ubo.projection = projection;
	ubo.projection[1][1] *= -1;

	auto device = game->getGraphics()->getLogicalDevice()->getDevice();
	void* data;
	vkMapMemory(device, uniformBuffers[currentImage]->getMemory(), 0, sizeof(ubo), 0, &data);
	memcpy(data, &ubo, sizeof(ubo));
	vkUnmapMemory(device, uniformBuffers[currentImage]->getMemory());
}

void Cube::createUniformBuffers(size_t swapChainImageSize) {
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);
	uniformBuffers.resize(swapChainImageSize);

	for (size_t i = 0; i < swapChainImageSize; i++) {
        uniformBuffers.push_back(new UniformBuffer(game->getGraphics()->getLogicalDevice(), bufferSize));
	}
}

std::vector<Vertex> Cube::getVertices() {
	return vertices;
}

std::vector<uint32_t> Cube::getIndices() {
	return indices;
}