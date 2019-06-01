#pragma once

#include "scene.h"
#include "scene_objects/alpaca.h"

class MainScene : public Scene {
public:
    explicit MainScene(Game* game) : Scene(game) { }
    ~MainScene() override;

    void setup() override;
    void update() override;
    void onMouseButton(int button, int action, int mods);
    void onKeyDown(int key, int scancode, int mods);
    void drawUI() override;
    std::vector<Alpaca*> getAlpacas();
private:
    Alpaca* selectedAlpaca = nullptr;
    int score = 0;
    void loopAlpacas(bool nextOrPrevious);
};

