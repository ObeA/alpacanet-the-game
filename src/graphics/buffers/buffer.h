#pragma once


#include "../logical_device.h"

class Buffer {
public:
    Buffer(LogicalDevice* device, VkDeviceSize size, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags properties, void* data = nullptr);
    ~Buffer();

    void* mapped;

    void copyFrom(void* data, uint32_t length);
    void copyFrom(void * data, uint32_t length, void * dst);
    void copyTo(Buffer* other);

    const VkBuffer& getBuffer() const;
    const VkDeviceMemory& getMemory() const;

    VkResult mapAll();
    void unmap();
    VkResult flush();

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

