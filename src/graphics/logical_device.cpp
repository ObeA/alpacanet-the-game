#include "logical_device.h"

LogicalDevice::LogicalDevice(Instance* instance, PhysicalDevice* physicalDevice, Surface* surface)
        : instance(instance),
          physicalDevice(physicalDevice),
          surface(surface) {
    createLogicalDevice();
    createCommandPool();
}

LogicalDevice::~LogicalDevice() {
    vkDestroyCommandPool(device, commandPool, nullptr);
    vkDestroyDevice(device, nullptr);
}

void LogicalDevice::createCommandPool() {
    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = physicalDevice->getGraphicsFamilyIndex();
    poolInfo.flags = 0; // Optional

    if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
}

void LogicalDevice::createLogicalDevice() {
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {physicalDevice->getGraphicsFamilyIndex(), physicalDevice->getPresentFamilyIndex()};

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
    deviceFeatures.sampleRateShading = VK_TRUE;

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

    vkGetDeviceQueue(device, physicalDevice->getGraphicsFamilyIndex(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, physicalDevice->getPresentFamilyIndex(), 0, &presentQueue);
}

VkImageView LogicalDevice::createImageView(const VkImage& image, VkFormat format, VkImageAspectFlags aspectFlags) const {
    VkImageViewCreateInfo viewInfo = {};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }

    return imageView;
}

VkShaderModule LogicalDevice::createShaderModule(const std::vector<char>& code) {
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
}

const VkDevice& LogicalDevice::getDevice() const {
    return device;
}

const VkQueue& LogicalDevice::getGraphicsQueue() const {
    return graphicsQueue;
}

const VkQueue& LogicalDevice::getPresentQueue() const {
    return presentQueue;
}

const VkCommandPool& LogicalDevice::getCommandPool() const {
    return commandPool;
}

PhysicalDevice* LogicalDevice::getPhysicalDevice() {
    return physicalDevice;
}

VkSampleCountFlagBits LogicalDevice::getSampleCount() const {
    return physicalDevice->getMaxUsableSampleCount();
}
