#pragma once


#include "../logical_device.h"
#include "../graphics.h"

class Buffer {
public:
    Buffer(Graphics* graphics, VkDeviceSize size, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags properties);
    ~Buffer();

protected:


private:
    Graphics* graphics;

    VkBuffer buffer;
    VkDeviceMemory memory;

    VkDeviceSize size;
    VkBufferUsageFlags usageFlags;
    VkMemoryPropertyFlags propertyFlags;

    void createBuffer();
};

