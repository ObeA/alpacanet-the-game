#pragma once

#include "instance.h"
#include "surface.h"
#include "physical_device.h"
#include "logical_device.h"
#include "renderer.h"

class Renderer;

class Graphics {
public:
    Graphics();
    ~Graphics();

    Window* getWindow();
    LogicalDevice* getLogicalDevice();
    PhysicalDevice* getPhysicalDevice();
    Renderer* getRenderer();
private:
    Instance* instance;
    Window* window;
    Surface* surface;
    PhysicalDevice* physicalDevice;
    LogicalDevice* logicalDevice;
    Renderer* renderer;
};

