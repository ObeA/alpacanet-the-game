#include "game_object.h"

GameObject::GameObject(Game* game, glm::vec3 position) : game(game), position(position) {

}

GameObject::~GameObject() {
}

bool GameObject::isStarted() const {
    return started;
}

void GameObject::start() {
    started = true;
}
