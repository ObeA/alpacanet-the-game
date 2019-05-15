#include "graphics.h"

Graphics::Graphics()
        : instance(),
          window(),
          physicalDevice(&instance, &surface),
          surface(&instance, &window, &physicalDevice),
          logicalDevice(&instance, &physicalDevice, &surface),
          renderer(&window, &surface, &logicalDevice, &physicalDevice) {

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

Renderer* Graphics::getRenderer() {
    return &renderer;
}
