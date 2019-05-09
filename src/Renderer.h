//
// Created by Leon on 5/8/2019.
//

#ifndef ALPACAWEB_RENDERER_H
#define ALPACAWEB_RENDERER_H

#include "Window.h"

class Window;

class Renderer {
public:
	Renderer(Window *window) : window(window) {
	}

	VkRenderPass renderPass;

	VkRenderPass offscreenRenderPass;

	std::vector<VkFramebuffer> swapChainFramebuffers;
	std::vector<VkImageView> swapChainImageViews;
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkExtent2D swapChainExtent;
	VkFormat swapChainImageFormat;

    VkFramebuffer offscreenFrameBuffer;


	void InitializeRenderer();
	void cleanup();
private:
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

	Window* window;
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


#endif //ALPACAWEB_RENDERER_H
