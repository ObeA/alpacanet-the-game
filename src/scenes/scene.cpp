#include "scene.h"

std::vector<GameObject *> Scene::getActiveObjects() {
    return objects;
}

Camera *Scene::getCamera() {
    return camera;
}
