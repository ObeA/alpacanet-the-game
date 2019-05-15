#pragma once

#include "buffer.h"

class UniformBuffer : public Buffer {
public:
    UniformBuffer(LogicalDevice* device, uint32_t size)
        : Buffer(
                device,
                size,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) { }
};