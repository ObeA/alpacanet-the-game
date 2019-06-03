#include "graphics.h"

Graphics::Graphics() {
    window = new Window;
    instance = new Instance;
    physicalDevice = new PhysicalDevice(instance);
    surface = new Surface(instance, window, physicalDevice);
    physicalDevice->setQueueFamilies(surface);
    logicalDevice = new LogicalDevice(instance, physicalDevice, surface);
    renderer = new Renderer(window, surface, logicalDevice);
}

Graphics::~Graphics() {
    delete physicalDevice;
    delete renderer;
    delete surface;
    delete window;
    delete logicalDevice;
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
