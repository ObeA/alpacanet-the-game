#pragma once

#include "scene.h"

class Material;

class MainScene : public Scene {
public:
    explicit MainScene(Game* game) : Scene(game) { }
    ~MainScene() override;

    void setup() override;
    void update() override;
private:
    std::vector<Material*> materials;
};

