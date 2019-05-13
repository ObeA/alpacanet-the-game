#pragma once


#include "../logical_device.h"
#include "../graphics.h"

class Buffer {
public:
    Buffer(LogicalDevice* device, VkDeviceSize size, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags properties, void* data = nullptr);
    ~Buffer();

    void copyTo(Buffer* other);

private:
    LogicalDevice* device;

    VkBuffer buffer;
    VkDeviceMemory memory;

    VkDeviceSize size;
    VkBufferUsageFlags usageFlags;
    VkMemoryPropertyFlags propertyFlags;

    void createBuffer();
    void copyFrom(void* data);
};

