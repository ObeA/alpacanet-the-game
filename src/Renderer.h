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

	std::vector<VkFramebuffer> swapChainFramebuffers;
	std::vector<VkImageView> swapChainImageViews;
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkExtent2D swapChainExtent;
	VkFormat swapChainImageFormat;


	void InitializeRenderer();
	void cleanup();
private:
	Window* window;
	void initializeRenderPass();
	VkFormat findDepthFormat();
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	void createFramebuffers();
	void createImageViews();
	void createSwapChain();
};


#endif //ALPACAWEB_RENDERER_H
