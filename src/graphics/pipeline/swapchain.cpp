#include "swapchain.h"

Swapchain::Swapchain(LogicalDevice* logicalDevice, Surface* surface, VkExtent2D extents)
        : logicalDevice(logicalDevice),
          surface(surface),
          physicalDevice(logicalDevice->getPhysicalDevice()),
          extents(extents) {
    createSwapchain();
}

Swapchain::~Swapchain() {
    vkDestroySwapchainKHR(logicalDevice->getDevice(), swapchain, nullptr);
}

void Swapchain::createSwapchain() {
    VkSurfaceFormatKHR surfaceFormat = surface->getFormat();
    VkPresentModeKHR presentMode = surface->getPresentMode();
    auto capabilities = surface->getCapabilities();

    uint32_t imageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
        imageCount = capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface->getSurface();

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extents;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    uint32_t queueFamilyIndices[] = {physicalDevice->getGraphicsFamilyIndex(), physicalDevice->getPresentFamilyIndex()};

    if (physicalDevice->getGraphicsFamilyIndex() != physicalDevice->getPresentFamilyIndex()) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    }

    createInfo.preTransform = capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(logicalDevice->getDevice(), &createInfo, nullptr, &swapchain) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create swapchain!");
    }

    vkGetSwapchainImagesKHR(logicalDevice->getDevice(), swapchain, &imageCount, nullptr);
    swapchainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(logicalDevice->getDevice(), swapchain, &imageCount, swapchainImages.data());
}

const VkSwapchainKHR& Swapchain::getSwapchain() const {
    return swapchain;
}

const VkExtent2D& Swapchain::getExtents() const {
    return extents;
}

const std::vector<VkImage>& Swapchain::getImages() const {
    return swapchainImages;
}
