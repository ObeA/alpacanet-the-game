#include "graphics.h"

Graphics::Graphics()
        : instance(),
          window(),
          physicalDevice(&instance, &surface),
          surface(&instance, &window, &physicalDevice),
          logicalDevice(&instance, &physicalDevice, &surface) {

}

Graphics::~Graphics() {

}

Window* Graphics::getWindow() {
    return &window;
}

LogicalDevice* Graphics::getLogicalDevice() {
    return &logicalDevice;
}

PhysicalDevice* Graphics::getPhysicalDevice() {
    return &physicalDevice;
}
