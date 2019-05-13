#pragma once

#include "../game_objects/game_object.h"
#include "../game_objects/camera.h"

class Scene {
public:
    virtual ~Scene() =0;

    virtual void setup() =0;

    std::vector<GameObject*> getActiveObjects();
    Camera* getCamera();

protected:
    std::vector<GameObject*> objects;
    Camera* camera;
};

