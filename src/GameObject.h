//
// Created by Leon on 4/28/2019.
//

#ifndef ALPACAWEB_GAMEOBJECT_H
#define ALPACAWEB_GAMEOBJECT_H

#include "Window.h"

class Window;

struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};


class GameObject {
public:
    GameObject(Window *window, Material *material) : window(window), material(material) {
    }

    virtual void generate(size_t swapchainImageSize);

    virtual void updateUniformBuffer(uint32_t currentImage, glm::mat4 perspective) = 0;

    virtual void cleanup(size_t swapchainImages);

    virtual void draw(VkCommandBuffer cmdbuffer, VkPipelineLayout pipelineLayout, size_t bufferOffset);

	virtual std::vector<Vertex> getVertices();

	virtual std::vector<uint16_t> getIndices();

	glm::vec3 position = { 0.0f,0.0f,0.0f };

protected:
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<VkDescriptorSet> descriptorSets;

	virtual void createVertexBuffer();

	virtual void createIndexBuffer();

	virtual void createUniformBuffers(size_t swapChainImageSize) = 0;

	virtual void createDescriptorSet(size_t swapChainImageSize) = 0;

    Window *window;

    Material *material;
private:
	std::vector<Vertex> vertices;
	std::vector<uint16_t> indices;
};


#endif //ALPACAWEB_GAMEOBJECT_H
