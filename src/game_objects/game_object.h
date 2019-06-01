#pragma once

#include "../game.h"

class Game;

class GameObject {
public:
    GameObject(Game* game, glm::vec3 position = glm::vec3(0.0f));
    ~GameObject();

	virtual void start() = 0;

	glm::vec3 position;
	glm::vec3 scale = glm::vec3(1.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
protected:
    Game* game;
};