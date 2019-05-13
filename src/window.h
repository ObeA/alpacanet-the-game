#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <map>
#include <optional>
#include <functional>
#include <cstdlib>
#include <set>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <array>
#include <cstring>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "models/vertex.h"
#include "utils/file_utilities.h"

class GameObject;
class Material;
class ShadowMaterial;
class Renderer;

const int WIDTH = 800;
const int HEIGHT = 600;

const int MAX_FRAMES_IN_FLIGHT = 2;

struct FrameBufferAttachment {
	VkImage image;
	VkDeviceMemory mem;
	VkImageView view;
};

struct UniformBufferObjectOffscreen {
    glm::mat4 depthVP;
	glm::mat4 model;
};

class Window {
public:
    Window();
    ~Window();

    GLFWwindow* window;

	VkDescriptorPool descriptorPool;

	Renderer* renderer;

	void run();

	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    VkExtent2D getSwapChainExtent();

    VkRenderPass& getRenderPass();

	void updateLight();

	VkRenderPass& getOffscreenRenderPass();

	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

	void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	VkExtent2D getExtents() const;

private:
    VkExtent2D extents;

	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	size_t currentFrame = 0;

	std::vector<GameObject*> objects;
    std::vector<Material*> materials;

    glm::vec3 lightPos = glm::vec3(0.0f);

	bool framebufferResized = false;

	void drawFrame();

	void initWindow();

	void initVulkan();

	void recreateSwapChain();

	void mainLoop();

	void cleanupSwapChain();

	void cleanup();

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

	VkShaderModule createShaderModule(const std::vector<char>& code);

	void createCommandPool();

	void createCommandBuffers();

	void createSyncObjects();

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	void createDescriptorPool();

	VkCommandBuffer beginSingleTimeCommands();

	void endSingleTimeCommands(VkCommandBuffer commandBuffer);

	bool hasStencilComponent(VkFormat format);
};