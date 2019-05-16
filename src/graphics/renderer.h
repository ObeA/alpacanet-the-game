#pragma once

#include "logical_device.h"
#include "pipeline/swapchain.h"
#include "../scenes/scene.h"

class Scene;

class Renderer {
public:
	Renderer(Window* window, Surface* surface, LogicalDevice* logicalDevice);
	~Renderer();

	std::vector<VkFramebuffer> swapChainFramebuffers;
	std::vector<VkImageView> swapChainImageViews;

    VkFramebuffer offscreenFrameBuffer;

    void setScene(Scene* scene);

    Swapchain* getSwapchain();
    const VkDescriptorPool& getDescriptorPool() const;
    const VkRenderPass& getRenderPass() const;
    const VkRenderPass& getOffscreenRenderPass() const;
private:
    Window* window;
    Surface* surface;
    LogicalDevice* logicalDevice;
    Scene* scene;

    Swapchain swapchain;

	VkImageView depthImageView;
	VkImage depthImage;
	VkDeviceMemory depthImageMemory;

    VkRenderPass renderPass;
    VkRenderPass offscreenRenderPass;

	VkImageView offscreenDepthImageView;
	VkImage offscreenDepthImage;
	VkDeviceMemory offscreenDepthImageMemory;
	VkSampler offscreenDepthSampler;
	VkImageView offscreenImageView;
	VkImage offscreenImage;
	VkDeviceMemory offscreenImageMemory;

	VkDescriptorPool descriptorPool;
    std::vector<VkCommandBuffer> commandBuffers;

    void createDescriptorPool();

	void initializeRenderPass();
	void initializeOffscreenRenderPass();
	VkFormat findDepthFormat();
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	void createFramebuffers();
	void createOffscreenFramebuffers();
	void createImageViews();
	void createDepthResources();
	void createDepthResourcesOffscreen();

	void createCommandbuffers();
};
