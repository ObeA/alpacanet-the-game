#pragma once

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
    explicit PhysicalDevice(Instance* instance);

    VkPhysicalDevice getDevice() const;

    uint32_t getGraphicsFamilyIndex() const;
    uint32_t getPresentFamilyIndex() const;
    VkSampleCountFlagBits getMaxUsableSampleCount() const;

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

    void setQueueFamilies(Surface* surface);

private:
    Instance* instance;
    QueueFamilyIndices indices;
    VkSampleCountFlagBits sampleCount;

    VkPhysicalDevice physicalDevice;

    void pickPhysicalDevice();
    QueueFamilyIndices findQueueFamilies(VkSurfaceKHR surface);



    VkSampleCountFlagBits findMaxUsableSampleCount();
};

