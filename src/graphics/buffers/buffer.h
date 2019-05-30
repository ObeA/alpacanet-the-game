#pragma once


#include "../logical_device.h"

class Buffer {
public:
    Buffer(LogicalDevice* device, VkDeviceSize size, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags properties, void* data = nullptr);
    ~Buffer();

    void copyFrom(void* data);
    void copyTo(Buffer* other);

    void* map();
    void unmap();

    const VkBuffer& getBuffer() const;
    const VkDeviceMemory& getMemory() const;

private:
    LogicalDevice* device;

    VkBuffer buffer;
    VkDeviceMemory memory;

    VkDeviceSize size;
    VkBufferUsageFlags usageFlags;
    VkMemoryPropertyFlags propertyFlags;

    void* mapped = nullptr;

    void createBuffer();
};

