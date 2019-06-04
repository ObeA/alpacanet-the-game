#pragma once

#include "game_object.h"

class Light : public GameObject {
public:
    Light(Game* game, glm::vec3 position, glm::vec3 target, float fov, float near, float far);

    void start() override { }
    void update() override { }

    const glm::mat4 getProjectionMatrix() const;
    const glm::mat4 getViewMatrix(const glm::vec3& up) const;
private:
    float fov;
    float near;
    float far;

    glm::vec3 target;
};

