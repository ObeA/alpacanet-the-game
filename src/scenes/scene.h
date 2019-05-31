#pragma once

#include <vector>
#include <typeinfo>

#include "../game_objects/camera.h"

class Camera;
class Game;
class GameObject;
class DrawableObject;

class Scene {
public:
    explicit Scene(Game* game) : game(game) { }
    virtual ~Scene() { }

    virtual void setup() =0;
    virtual void update();

    std::vector<GameObject*> getActiveObjects();
    std::vector<DrawableObject*> getActiveDrawableObjects();
    Camera* getCamera();
    virtual void drawUI() = 0;

protected:
    Game* game;

    std::vector<GameObject*> objects;
    Camera* camera{};
};

