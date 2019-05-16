#pragma once

#include "scene.h"

class MainScene : public Scene {
public:
    explicit MainScene(Game* game) : Scene(game) { }
    ~MainScene() override;

    void setup() override;
};

