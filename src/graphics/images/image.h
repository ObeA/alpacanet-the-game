#pragma once


#include "../logical_device.h"
#include "../buffers/buffer.h"

class Image {
public:
    Image(LogicalDevice* logicalDevice, VkExtent2D extents, VkFormat format, VkImageTiling tiling,
          VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
    ~Image();

    void transitionLayout(VkImageLayout oldLayout, VkImageLayout newLayout);
    void copyFromBuffer(Buffer* buffer);

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
