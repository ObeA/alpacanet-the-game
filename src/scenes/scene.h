#pragma once

#include <vector>
#include <typeinfo>

#include "../game_objects/camera.h"

class Game;
class GameObject;
class DrawableObject;

class Scene {
public:
    explicit Scene(Game* game) : game(game) { }
    virtual ~Scene() { }

    virtual void setup() =0;
    virtual void update() =0;

    std::vector<GameObject*> getActiveObjects();
    std::vector<DrawableObject*> getActiveDrawableObjects();
    Camera* getCamera();

protected:
    Game* game;

    std::vector<GameObject*> objects;
    Camera* camera{};
};

