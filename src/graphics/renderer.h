#pragma once

#include "logical_device.h"

class Renderer {
public:
	Renderer(Window* window, Surface* surface, LogicalDevice* logicalDevice, PhysicalDevice* physicalDevice);
	~Renderer();

	VkRenderPass renderPass;

	VkRenderPass offscreenRenderPass;

	std::vector<VkFramebuffer> swapChainFramebuffers;
	std::vector<VkImageView> swapChainImageViews;
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkExtent2D swapChainExtent;
	VkFormat swapChainImageFormat;

    VkFramebuffer offscreenFrameBuffer;
private:
    Window* window;
    Surface* surface;
    LogicalDevice* logicalDevice;
    PhysicalDevice* physicalDevice;

	VkImageView depthImageView;
	VkImage depthImage;
	VkDeviceMemory depthImageMemory;

	VkImageView offscreenDepthImageView;
	VkImage offscreenDepthImage;
	VkDeviceMemory offscreenDepthImageMemory;
	VkSampler offscreenDepthSampler;
	VkImageView offscreenImageView;
	VkImage offscreenImage;
	VkDeviceMemory offscreenImageMemory;

	void initializeRenderPass();
	void initializeOffscreenRenderPass();
	VkFormat findDepthFormat();
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	void createFramebuffers();
	void createOffscreenFramebuffers();
	void createImageViews();
	void createSwapChain();
	void createDepthResources();
	void createDepthResourcesOffscreen();
};
