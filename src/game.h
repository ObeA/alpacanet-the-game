#pragma once

#include "scenes/scene.h"
#include "graphics/graphics.h"

class Scene;
class Graphics;

class Game {
public:
    static constexpr std::chrono::nanoseconds Timestep = std::chrono::milliseconds(16);

    Game();
    ~Game();

    void run();

    Scene* getCurrentScene();
    Graphics* getGraphics();

    unsigned long long currentTick() const;
private:
    Graphics* graphics;

    std::vector<Scene*> scenes;
    Scene* current_scene;

    void setup();

    unsigned long long tick;
};
