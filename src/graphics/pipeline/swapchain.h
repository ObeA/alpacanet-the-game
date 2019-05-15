#pragma once


#include "../logical_device.h"

class Swapchain {
public:
    Swapchain(LogicalDevice* logicalDevice, Surface* surface, VkExtent2D extents);
    ~Swapchain();

    const VkSwapchainKHR& getSwapchain() const;
    const VkExtent2D& getExtents() const;
    const std::vector<VkImage>& getImages() const;
private:
    LogicalDevice* logicalDevice;
    Surface* surface;
    PhysicalDevice* physicalDevice;

    VkSwapchainKHR swapchain;
    std::vector<VkImage> swapchainImages;
    VkExtent2D extents;

    void createSwapchain();
};

