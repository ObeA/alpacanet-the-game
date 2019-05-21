#include "alpaca.h"
#include "../../game_objects/model_object.h"
#include "../../materials/basic_material.h"
#include "../../materials/basic_textured_material.h"
#include "../../materials/shadow_material.h"

Alpaca::Alpaca(Game* game, Material* material, Material* shadowMaterial)
    : ModelObject(game, material, shadowMaterial, "assets/models/cube.obj") {
}

void Alpaca::start() {
    ModelObject::start();
    wooliness = (rand() % 50);
}

void Alpaca::moveTo(glm::vec2 position)
{
    targetPosition = position;
    targetPositionReached = false;
}

float moveTowards(float from, float to, float amount) {
    float result = 0;
    if (from < to)
        result = std::min(from + amount, to);
    else if (from > to)
        result = std::max(from - amount, to);
    else
        result = to;
    return result;
}

void Alpaca::update() {
    if (rand() % 500 == 1) {
        wooliness++;
    }

    auto angle = std::atan2(targetPosition.y - position.y, targetPosition.x - position.x);
    rotation.y = angle;

    if (!targetPositionReached) {
        if (glm::distance(glm::vec2(position), targetPosition) < 0.1) {
            targetPositionReached = true;
        }
        else {
            auto heading = glm::vec2(std::cos(angle), std::sin(angle));
            position.x += heading.x * 0.01;
            position.y += heading.y * 0.01;
        }
        if (bounceCompleted) {
            bounceCompleted = false;
            bounceStartTime = std::chrono::high_resolution_clock::now();
        }
    }

    if (!bounceCompleted) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - bounceStartTime).count();
        float newPos = std::sin(time * 2);
        if (newPos < 0) {
            newPos = 0;
            bounceCompleted = true;
        }
        position.z = newPos;
    }
}

int Alpaca::shear() {
    auto temp = wooliness;
    wooliness = 0;
    return temp;
}