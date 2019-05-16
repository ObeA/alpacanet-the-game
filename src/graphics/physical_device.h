#pragma once

#include <vulkan/vulkan.h>
#include "instance.h"
#include "surface.h"

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() const {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

class Surface;

class PhysicalDevice {
public:
    PhysicalDevice(Instance* instance, Surface* surface);

    VkPhysicalDevice getDevice() const;

    uint32_t getGraphicsFamilyIndex() const;
    uint32_t getPresentFamilyIndex() const;

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

private:
    Instance* instance;
    Surface* surface;
    QueueFamilyIndices indices;

    VkPhysicalDevice physicalDevice;

    void pickPhysicalDevice();
    QueueFamilyIndices findQueueFamilies();
};

