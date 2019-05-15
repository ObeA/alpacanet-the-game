#include "surface.h"
#include "physical_device.h"

Surface::Surface(Instance* instance, Window* window, PhysicalDevice* physicalDevice)
        : instance(instance),
          window(window),
          physicalDevice(physicalDevice),
          surface(VK_NULL_HANDLE),
          format{} {
    createSurface();
    format = getBestFormat();
    presentMode = getBestPresentMode();
    capabilities = fetchCapabilities();
}

Surface::~Surface() {
    vkDestroySurfaceKHR(instance->getInstance(), surface, nullptr);
}

void Surface::createSurface() {
    if (glfwCreateWindowSurface(instance->getInstance(), window->window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}

VkSurfaceKHR Surface::getSurface() const {
    return surface;
}

VkPresentModeKHR Surface::getPresentMode() {
    return presentMode;
}

VkSurfaceFormatKHR Surface::getFormat() const {
    return format;
}

bool Surface::hasLimitedExtents() const {
    return capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max();
}

VkSurfaceCapabilitiesKHR Surface::getCapabilities() const {
    return capabilities;
}

void Surface::fetchCapabilities() {
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice->getDevice(), surface, &capabilities);
}

VkSurfaceFormatKHR Surface::getBestFormat() const {
    auto device = physicalDevice->getDevice();

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    std::vector<VkSurfaceFormatKHR> availableFormats(formatCount);
    if (formatCount == 0
        || vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, availableFormats.data()) != VK_SUCCESS) {
        throw std::runtime_error("Device doesn't support any format");
    }

    if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
        return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    }

    for (const auto& availableFormat : availableFormats) {
        if (format.format == VK_FORMAT_B8G8R8A8_UNORM
            && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return format;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR Surface::getBestPresentMode() const {
    auto device = physicalDevice->getDevice();

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    std::vector<VkPresentModeKHR> availablePresentModes(presentModeCount);
    if (presentModeCount == 0
         || vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, availablePresentModes.data()) != VK_SUCCESS) {
        throw std::runtime_error("Device doesn't support present mode");
    }

    VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        } else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
            bestMode = availablePresentMode;
        }
    }

    return bestMode;
}
