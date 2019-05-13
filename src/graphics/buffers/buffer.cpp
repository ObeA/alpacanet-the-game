#include "buffer.h"
#include "../graphics.h"

Buffer::Buffer(Graphics* graphics,
               VkDeviceSize size,
               VkBufferUsageFlags usageFlags,
               VkMemoryPropertyFlags propertyFlags)
        : graphics(graphics),
          size(size),
          usageFlags(usageFlags),
          propertyFlags(propertyFlags),
          buffer(VK_NULL_HANDLE),
          memory(VK_NULL_HANDLE) {
    createBuffer();
}

Buffer::~Buffer() {
    auto device = graphics->getLogicalDevice()->getDevice();

    vkDestroyBuffer(device, buffer, nullptr);
    vkFreeMemory(device, memory, nullptr);
}

void Buffer::createBuffer() {
    auto physicalDevice = graphics->getPhysicalDevice();
    auto device = graphics->getLogicalDevice();

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