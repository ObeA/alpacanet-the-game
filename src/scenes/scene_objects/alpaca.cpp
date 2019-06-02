#include "alpaca.h"
#include "../../game_objects/model_object.h"
#include "../../materials/basic_material.h"
#include "../../materials/basic_textured_material.h"
#include "../../materials/shadow_material.h"
#include "../../utils/random_utilities.h"

Alpaca::Alpaca(Game* game, Material* material, Material* shadowMaterial)
    : ModelObject(game, material, shadowMaterial, "assets/models/alpaca.obj"), nextMoveTick(0), wooliness(0), bouncyBoi(0) {
    age = RandomUtilities::getInstance().getRandomBetween(MIN_AGE, MAX_AGE);
    updateAge();
}

void Alpaca::start() {
    ModelObject::start();
}

void Alpaca::moveTo(glm::vec2 position) {
    targetPosition = position;
    targetPositionReached = false;
}

void Alpaca::update() {
    if (std::rand() % 50 == 1) {
        wooliness++;
    }

    auto angle = std::atan2(targetPosition.y - position.y, targetPosition.x - position.x);
    rotation.z = angle + M_PI;

    if (!targetPositionReached) {
        if (glm::distance(glm::vec2(position), targetPosition) < 0.1) {
            targetPositionReached = true;
        } else {
            auto heading = glm::vec2(std::cos(angle), std::sin(angle));
            position.x += heading.x * 0.01f;
            position.y += heading.y * 0.01f;
        }

        if (bounceCompleted) {
            bounceCompleted = false;
            bouncyBoi = 0;
        }
    }

    if (!bounceCompleted) {
        bouncyBoi += 0.05f;
        float newPos = std::sin(bouncyBoi);
        if (newPos < 0) {
            newPos = 0;
            bounceCompleted = true;
        }

        position.z = newPos;
    }

    updateAge();
}

int Alpaca::shear() {
    auto temp = wooliness;
    wooliness = 0;
    return temp;
}

int Alpaca::getWooliness() {
    return wooliness;
}

void Alpaca::updateAge() {
    age = std::min(age + 0.001f, MAX_AGE);
    scale = BASE_SIZE * age;
}
