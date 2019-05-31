#pragma once

#include "logical_device.h"
#include "pipeline/swapchain.h"
#include "../scenes/scene.h"
#include "images/image.h"

class Scene;
class GUI;

class Renderer {
public:
    static const uint32_t SHADOWMAP_DIMENSION = 2048;

	Renderer(Window* window, Surface* surface, LogicalDevice* logicalDevice);
	~Renderer();

    void recreateCommandBuffer();

	std::vector<VkFramebuffer> swapChainFramebuffers;
	std::vector<VkImageView> swapChainImageViews;

    VkFramebuffer offscreenFrameBuffer;
    VkImageView offscreenDepthImageView;
    VkDeviceMemory offscreenDepthImageMemory;
    VkImage offscreenDepthImage;
    VkSampler offscreenDepthSampler;
    VkImageView offscreenImageView;
    Image* offscreenImage;

    void setScene(Scene* scene);

	void render();

    Swapchain* getSwapchain();
    const VkDescriptorPool& getDescriptorPool() const;
    const VkRenderPass& getRenderPass() const;
    const VkRenderPass& getOffscreenRenderPass() const;
    const VkExtent2D& getExtents() const;
    GUI* getGui();
private:

    const VkFilter SHADOWMAP_FILTER = VK_FILTER_LINEAR;
    const VkFormat DEPTH_FORMAT = VK_FORMAT_D16_UNORM;
    const glm::vec3 LIGHT_POSITION = glm::vec3(5, 5, 5);

    Window* window;
    Surface* surface;
    LogicalDevice* logicalDevice;
    Scene* scene;

    Swapchain swapchain;

	VkImageView depthImageView;
	Image* depthImage;

    VkRenderPass renderPass;
    VkRenderPass offscreenRenderPass;



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
	void createImageViews();
	void createDepthResources();

	void createCommandbuffers();

	void createSyncObjects();

    void initializeOffscreenRendering();
    void initializeOffscreenFramebuffer();

    void createGUI();
};
