#include "scene.h"
#include "../game_objects/drawable_object.h"

std::vector<GameObject*> Scene::getActiveObjects() {
    return objects;
}

std::vector<DrawableObject*> Scene::getActiveDrawableObjects() {
    auto drawableObjects = std::vector<DrawableObject*>();
    for (auto object : objects) {
        auto casted = dynamic_cast<DrawableObject*>(object);
        if (casted != nullptr) {
            drawableObjects.push_back(casted);
        }
    }
    return drawableObjects;
}

Camera* Scene::getCamera() {
    return camera;
}

void Scene::update() {
    camera->update();
    for (auto gameObject : objects)
    {
        gameObject->update();
    }
}
