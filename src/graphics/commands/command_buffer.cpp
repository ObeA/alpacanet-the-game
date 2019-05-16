#include "command_buffer.h"

CommandBuffer::CommandBuffer(LogicalDevice* device,
        VkCommandBufferUsageFlags flags) : device(device), flags(flags) {
    createBuffer();
}

void CommandBuffer::createBuffer() {
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = device->getCommandPool();
    allocInfo.commandBufferCount = 1;

    vkAllocateCommandBuffers(device->getDevice(), &allocInfo, &buffer);
}

CommandBuffer& CommandBuffer::begin() {
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = flags;

    vkBeginCommandBuffer(buffer, &beginInfo);

    return *this;
}

CommandBuffer& CommandBuffer::end() {
    vkEndCommandBuffer(buffer);

    return *this;
}

CommandBuffer& CommandBuffer::submit() {
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &buffer;

    vkQueueSubmit(device->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);

    return *this;
}

CommandBuffer& CommandBuffer::wait() {
    vkQueueWaitIdle(device->getGraphicsQueue());

    return *this;
}

CommandBuffer::~CommandBuffer() {
    vkFreeCommandBuffers(device->getDevice(), device->getCommandPool(), 1, &buffer);
}

CommandBuffer::operator const VkCommandBuffer&() const {
    return getCommandBuffer();
}

const VkCommandBuffer& CommandBuffer::getCommandBuffer() const {
    return buffer;
}
