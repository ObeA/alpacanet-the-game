#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>
#include <vector>

class Instance {
public:
    static const bool EnableValidationLayers;
    static const std::vector<const char*> ValidationLayers;
    static const std::vector<const char*> DeviceExtensions;

    Instance();

    ~Instance();

    const VkInstance& getInstance() const;

private:
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger{};

    void createInstance();

    std::vector<const char*> getRequiredExtensions() const;

    bool checkValidationLayerSupport() const;

    void setupDebugMessenger();

    static VkBool32 debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                  VkDebugUtilsMessageTypeFlagsEXT messageType,
                                  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                  void* pUserData
    );
};

