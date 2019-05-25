#include "buffer.h"
#include "../graphics.h"

Buffer::Buffer(LogicalDevice* device,
               VkDeviceSize size,
               VkBufferUsageFlags usageFlags,
               VkMemoryPropertyFlags propertyFlags,
               void* data)
        : device(device),
          size(size),
          usageFlags(usageFlags),
          propertyFlags(propertyFlags),
          buffer(VK_NULL_HANDLE),
          memory(VK_NULL_HANDLE) {
    createBuffer();

    if (data != nullptr) {
        copyFrom(data);
    }
}

Buffer::~Buffer() {
    unmap();
    vkDestroyBuffer(device->getDevice(), buffer, nullptr);
    vkFreeMemory(device->getDevice(), memory, nullptr);
}

void Buffer::createBuffer() {
    auto physicalDevice = device->getPhysicalDevice();

    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usageFlags;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device->getDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(device->getDevice(), buffer, &memoryRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memoryRequirements.size;
    allocInfo.memoryTypeIndex = physicalDevice->findMemoryType(memoryRequirements.memoryTypeBits,
                                                               propertyFlags);

    if (vkAllocateMemory(device->getDevice(), &allocInfo, nullptr, &memory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(device->getDevice(), buffer, memory, 0);
}

VkResult Buffer::mapAll() {
    return vkMapMemory(device->getDevice(), memory, 0, VK_WHOLE_SIZE, 0, &mapped);
}

void Buffer::unmap()
{
    if (mapped != nullptr)
    {
        vkUnmapMemory(device->getDevice(), memory);
        mapped = nullptr;
    }
}

VkResult Buffer::flush()
{
    VkMappedMemoryRange mappedRange = {};
    mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    mappedRange.memory = memory;
    mappedRange.offset = 0;
    mappedRange.size = VK_WHOLE_SIZE;
    return vkFlushMappedMemoryRanges(device->getDevice(), 1, &mappedRange);
}

void Buffer::copyFrom(void* data) {
    copyFrom(data, size);
}

void Buffer::copyFrom(void* data, uint32_t length) {
    void* mappedMemory;
    vkMapMemory(device->getDevice(), memory, 0, length, 0, &mappedMemory);
    memcpy(mappedMemory, data, length);
    vkUnmapMemory(device->getDevice(), memory);
}

void Buffer::copyTo(Buffer* other) {
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = device->getCommandPool();
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device->getDevice(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkBufferCopy copy = {};
    copy.size = size;
    vkCmdCopyBuffer(commandBuffer, buffer, other->buffer, 1, &copy);

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(device->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(device->getGraphicsQueue());

    vkFreeCommandBuffers(device->getDevice(), device->getCommandPool(), 1, &commandBuffer);
}

const VkBuffer& Buffer::getBuffer() const {
    return buffer;
}

const VkDeviceMemory& Buffer::getMemory() const {
    return memory;
}
