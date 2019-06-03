#pragma once

#include "physical_device.h"
#include "instance.h"
#include "surface.h"

class LogicalDevice {
public:
    LogicalDevice(Instance* instance, PhysicalDevice* physicalDevice, Surface* surface);
    ~LogicalDevice();

    PhysicalDevice* getPhysicalDevice();

    const VkDevice& getDevice() const;
    const VkQueue& getGraphicsQueue() const;
    const VkQueue& getPresentQueue() const;
    const VkCommandPool& getCommandPool() const;

    VkImageView createImageView(const VkImage& image, VkFormat format, VkImageAspectFlags aspectFlags) const;
    VkShaderModule createShaderModule(const std::vector<char>& code);

    VkSampleCountFlagBits getSampleCount() const;
private:
    Instance* instance;
    PhysicalDevice* physicalDevice;
    Surface* surface;

    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkCommandPool commandPool;

    QueueFamilyIndices indices;

    void createLogicalDevice();
    void createCommandPool();
};

