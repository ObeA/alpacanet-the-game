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

	//void run();

	//void updateLight();

	//void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

	//void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	//void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

	VkExtent2D getExtents() const;

	void initWindow();

	bool shouldClose();

	void pollEvents();

private:
    VkExtent2D extents;

	std::vector<VkCommandBuffer> commandBuffers;

	//std::vector<VkSemaphore> imageAvailableSemaphores;
	//std::vector<VkSemaphore> renderFinishedSemaphores;
	//std::vector<VkFence> inFlightFences;
	//size_t currentFrame = 0;

    glm::vec3 lightPos = glm::vec3(0.0f);

	bool framebufferResized = false;

	//void drawFrame();

	//void mainLoop();

	//void cleanupSwapChain();

	//void cleanup();

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

	//VkShaderModule createShaderModule(const std::vector<char>& code);

	//void createCommandBuffers();

	//void createSyncObjects();
};