#pragma once

#include "../window.h"
#include "../game.h"
#include "../graphics/buffers/buffer.h"

//struct UniformBufferObject {
//    alignas(16) glm::mat4 model;
//    alignas(16) glm::mat4 view;
//    alignas(16) glm::mat4 proj;
//};

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 depthBiasMVP;
    glm::vec3 lightPos;
};

class Game;

class GameObject {
public:
    GameObject(Game* game, Material* material, Material* shadowMaterial, size_t swapchainImageSize);
    ~GameObject();

	std::vector<VkDescriptorSet> descriptorSets;

	VkDescriptorSet offscreenDescriptorSets;

    virtual void generate(size_t swapchainImageSize);

    virtual void updateUniformBuffer(uint32_t currentImage, glm::mat4 perspective, glm::vec3 lightPos) = 0;

    virtual void cleanup(size_t swapchainImages);

    virtual void draw(VkCommandBuffer cmdbuffer, size_t bufferOffset);

	virtual std::vector<Vertex> getVertices();

	virtual std::vector<uint32_t> getIndices();

	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	Material *material;
	Material *shadowMaterial;

protected:
    Game* game;

	Buffer* vertexBuffer;
	Buffer* indexBuffer;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	VkBuffer offscreenUniformBuffer;
	VkDeviceMemory offscreenUniformBuffersMemory;

    size_t swapchainImageSize;

	virtual void createVertexBuffer();

	virtual void createIndexBuffer();

	virtual void createUniformBuffers(size_t swapChainImageSize) = 0;

	void createDescriptorSet(size_t swapChainImageSize);

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};