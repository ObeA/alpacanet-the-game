#pragma once

#include "../game_objects/game_object.h"
#include "../game_objects/camera.h"

class Camera;
class Game;
class GameObject;

class Scene {
public:
    explicit Scene(Game* game) : game(game) { }
    virtual ~Scene() { }

    virtual void setup() =0;
    virtual void update() =0;

    std::vector<GameObject*> getActiveObjects();
    Camera* getCamera();

protected:
    Game* game;

    std::vector<GameObject*> objects;
    Camera* camera{};
};

