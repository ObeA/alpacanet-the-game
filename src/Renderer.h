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


	void InitializeRenderer();
	void cleanup();
private:
	Window* window;
	void initializeRenderPass();
	VkFormat findDepthFormat();
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	void createFramebuffers();
};


#endif //ALPACAWEB_RENDERER_H
