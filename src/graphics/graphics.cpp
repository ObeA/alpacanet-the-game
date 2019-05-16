#include "graphics.h"

Graphics::Graphics()
        : instance(),
          window(),
          physicalDevice(new PhysicalDevice(instance, surface)),
          surface(new Surface(instance, window, physicalDevice)),
          logicalDevice(new LogicalDevice(instance, physicalDevice, surface)),
          renderer(new Renderer(window, surface, logicalDevice)) {

}

Graphics::~Graphics() {
    delete window;
    delete physicalDevice;
    delete surface;
    delete logicalDevice;
    delete renderer;
    delete instance;
}

Window* Graphics::getWindow() {
    return window;
}

LogicalDevice* Graphics::getLogicalDevice() {
    return logicalDevice;
}

PhysicalDevice* Graphics::getPhysicalDevice() {
    return physicalDevice;
}

Renderer* Graphics::getRenderer() {
    return renderer;
}
