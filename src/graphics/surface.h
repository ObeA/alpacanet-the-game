#pragma once


#include "../window.h"
#include "instance.h"
#include "physical_device.h"

class Surface {
public:
    Surface(Instance* instance, Window* window, PhysicalDevice* physicalDevice);
    ~Surface();

    VkSurfaceKHR getSurface() const;
    VkSurfaceFormatKHR getFormat() const;
    VkPresentModeKHR getPresentMode();
    VkSurfaceCapabilitiesKHR getCapabilities() const;
    bool hasLimitedExtents() const;

private:
    Instance* instance;
    Window* window;
    PhysicalDevice* physicalDevice;

    VkSurfaceKHR surface;
    VkSurfaceFormatKHR format;
    VkPresentModeKHR presentMode;
    VkSurfaceCapabilitiesKHR capabilities;

    void createSurface();

    VkSurfaceFormatKHR getBestFormat() const;
    VkPresentModeKHR getBestPresentMode() const;
    void fetchCapabilities();
};

