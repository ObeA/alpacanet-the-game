#pragma once

#include "logical_device.h"
#include "pipeline/swapchain.h"
#include "../scenes/scene.h"
#include "images/image.h"

class Scene;
class GUI;

class Renderer {
public:
	Renderer(Window* window, Surface* surface, LogicalDevice* logicalDevice);
	~Renderer();

	std::vector<VkFramebuffer> swapChainFramebuffers;
	std::vector<VkImageView> swapChainImageViews;

    VkFramebuffer offscreenFrameBuffer;

    void setScene(Scene* scene);

	void render();

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
	Image* depthImage;

    VkRenderPass renderPass;
    VkRenderPass offscreenRenderPass;

	VkImageView offscreenDepthImageView;
	Image* offscreenDepthImage;
	VkSampler offscreenDepthSampler;
	VkImageView offscreenImageView;
	Image* offscreenImage;

    GUI* gui;

	VkDescriptorPool descriptorPool;
    std::vector<VkCommandBuffer> commandBuffers;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	size_t currentFrame = 0;

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

	void createSyncObjects();
};
