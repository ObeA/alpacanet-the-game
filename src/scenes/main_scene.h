#pragma once

#include "scene.h"

class Material;
class Alpaca;

class MainScene : public Scene {
public:
    explicit MainScene(Game* game) : Scene(game) { }
    ~MainScene() override;

    void setup() override;
    void update() override;
    void onMouseButton(int button, int action, int mods);
    void onKeyDown(int key, int scancode, int mods);
    std::vector<Alpaca*> getAlpacas();
private:
    Alpaca* selectedAlpaca = nullptr;
    int score = 0;
    void loopAlpacas(bool nextOrPrevious);
};

