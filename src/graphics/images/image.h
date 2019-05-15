#pragma once


#include "../logical_device.h"

class Image {
public:
    Image(LogicalDevice* logicalDevice, VkExtent2D extents, VkFormat format, VkImageTiling tiling,
          VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
    ~Image();

    const VkImage& getImage() const;
    const VkDeviceMemory& getMemory() const;
private:
    LogicalDevice* device;

    VkExtent2D extents;
    VkFormat format;

    VkImage image;
    VkDeviceMemory memory;

    void createImage(VkImageTiling tiling,  VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
};

