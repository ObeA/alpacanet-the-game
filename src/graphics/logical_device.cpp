#include "logical_device.h"

LogicalDevice::LogicalDevice(Instance* instance, PhysicalDevice* physicalDevice, Surface* surface)
        : instance(instance),
          physicalDevice(physicalDevice),
          surface(surface),
          indices(findQueueFamilies()) {
    createLogicalDevice();
}

LogicalDevice::~LogicalDevice() {
    vkDestroyDevice(device, nullptr);
}

void LogicalDevice::createLogicalDevice() {
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(Instance::DeviceExtensions.size());
    createInfo.ppEnabledExtensionNames = Instance::DeviceExtensions.data();

    if (Instance::EnableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(Instance::ValidationLayers.size());
        createInfo.ppEnabledLayerNames = Instance::ValidationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(physicalDevice->getDevice(), &createInfo, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}

const VkDevice& LogicalDevice::getDevice() const {
    return device;
}

const VkQueue& LogicalDevice::GetGraphicsQueue() const {
    return graphicsQueue;
}

const VkQueue& LogicalDevice::GetPresentQueue() const {
    return presentQueue;
}



