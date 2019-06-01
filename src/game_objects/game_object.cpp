#include "game_object.h"

GameObject::GameObject(Game* game, glm::vec3 position) : game(game), position(position) {

}

GameObject::~GameObject() {
}

void GameObject::update() {
}