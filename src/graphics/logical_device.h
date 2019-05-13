#pragma once

#include <vulkan/vulkan.h>
#include "physical_device.h"
#include "instance.h"
#include "surface.h"

class LogicalDevice {
public:
    LogicalDevice(Instance* instance, PhysicalDevice* physicalDevice, Surface* surface);
    ~LogicalDevice();

    VkDevice& getDevice() const;

    const VkQueue& GetGraphicsQueue() const;
    const VkQueue& GetPresentQueue() const;


private:
    Instance* instance;
    PhysicalDevice* physicalDevice;
    Surface* surface;

    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;

    QueueFamilyIndices indices;

    void createLogicalDevice();
};

