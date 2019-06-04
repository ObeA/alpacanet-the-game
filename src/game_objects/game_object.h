#pragma once

#include "../game.h"

class Game;

class GameObject {
public:
    explicit GameObject(Game* game, glm::vec3 position = glm::vec3(0.0f));
    virtual ~GameObject() = 0;

	virtual void start();
	bool isStarted() const;

    virtual void update() =0;

	glm::vec3 position;
	glm::vec3 scale = glm::vec3(1.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
protected:
    Game* game;
private:
    bool started = false;
};