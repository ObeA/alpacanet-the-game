#pragma once

#include "window.h"
#include "scenes/scene.h"
#include "graphics/instance.h"
#include "graphics/surface.h"
#include "graphics/physical_device.h"
#include "graphics/logical_device.h"
#include "graphics/graphics.h"

class Game {
public:
    Game();
    ~Game();

    void run();

    Scene* getCurrentScene();
    Graphics* getGraphics();
private:
    Graphics graphics;

    std::vector<Scene*> scenes;
    Scene* current_scene;

    void setup();
};
