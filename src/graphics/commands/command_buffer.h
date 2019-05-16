#pragma once


#include "../logical_device.h"

class CommandBuffer {
public:
    CommandBuffer(LogicalDevice* device, VkCommandBufferUsageFlags flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    ~CommandBuffer();

    CommandBuffer& begin();
    CommandBuffer& end();
    CommandBuffer& submit();
    CommandBuffer& wait();

    const VkCommandBuffer& getCommandBuffer() const;
    operator const VkCommandBuffer& () const;
private:
    LogicalDevice* device;

    VkCommandBufferUsageFlags flags;

    VkCommandBuffer buffer;

    void createBuffer();
};

