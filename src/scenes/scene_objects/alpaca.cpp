#include "alpaca.h"
#include "../../game_objects/model_object.h"
#include "../../materials/basic_material.h"
#include "../../materials/basic_textured_material.h"
#include "../../materials/shadow_material.h"
#include "../../utils/random_utilities.h"

Alpaca::Alpaca(Game* game, Material* material, Material* shadowMaterial)
    : ModelObject(game, material, shadowMaterial, "assets/models/alpaca.obj"),
      targetPositionReached(true), nextMoveTick(0), wooliness(0), happiness(0.5), bouncyBoi(0) {
    age = RandomUtilities::getInstance().getRandomBetween(MIN_AGE, MIN_AGE + (MAX_AGE - MIN_AGE) * 0.5f);
    updateAge();
}

void Alpaca::moveTo(glm::vec2 position) {
    targetPosition = position;
    targetPositionReached = false;
}

void Alpaca::update() {
    updatePosition();
    updateWool();
    updateAge();
    updateHappiness();
}

int Alpaca::shear() {
    auto temp = getWooliness();
    wooliness = 0;
    return temp;
}

int Alpaca::getWooliness() const {
    return (int)std::round(wooliness * (20.0f * getAgeScale()));
}

int Alpaca::getHappiness() const {
    return (int)std::round(happiness * 100.0f);
}

void Alpaca::updateAge() {
    age = std::min(age + 0.0005f * happiness, MAX_AGE);
    scale = BASE_SIZE + glm::vec3(0.25) * getAgeScale();
}

void Alpaca::updateWool() {
    wooliness = std::min(1.0f, wooliness + 0.001f * getAgeScale() * happiness);
}

void Alpaca::updateHappiness() {
    if (wooliness > 0.9f) {
        happiness -= 0.0005f * wooliness;
    }

    if (wooliness < 0.5f) {
        happiness += 0.001f * (1 - wooliness);
    }

    happiness = std::min(1.0f, std::max(0.0f, happiness));
}

void Alpaca::updatePosition() {
    auto angle = std::atan2(targetPosition.y - position.y, targetPosition.x - position.x);
    rotation.z = angle;

    if (!targetPositionReached) {
        if (glm::distance(glm::vec2(position), targetPosition) < 0.1) {
            targetPositionReached = true;
        } else {
            auto heading = glm::vec2(std::cos(angle), std::sin(angle));
            auto speed = 0.01f * happiness * getAgeScale();
            position.x += heading.x * speed;
            position.y += heading.y * speed;
        }

        if (bounceCompleted) {
            bounceCompleted = false;
            bouncyBoi = 0;
        }
    }

    if (!bounceCompleted) {
        bouncyBoi += 0.05f;
        float newPos = std::sin(bouncyBoi) * getAgeScale() * happiness;
        if (newPos < 0) {
            newPos = 0;
            bounceCompleted = true;
        }

        position.z = newPos;
    }
}

float Alpaca::getAgeScale() const {
    return (age - MIN_AGE) / (MAX_AGE - MIN_AGE);
}

bool Alpaca::hasReachedTargetPosition() const {
    return targetPositionReached;
}

void Alpaca::postprocessMaterials(std::vector<tinyobj::material_t>& materials) {
    auto color = COLORS[RandomUtilities::getInstance().getRandomBetween(0, COLORS.size() - 1)];
    for (auto& material : materials) {
        if (material.name == "Body") {
            material.diffuse[0] = color.body.r;
            material.diffuse[1] = color.body.g;
            material.diffuse[2] = color.body.b;
        } else if (material.name == "Cloth") {
            material.diffuse[0] = color.cloth.r;
            material.diffuse[1] = color.cloth.g;
            material.diffuse[2] = color.cloth.b;
        }
    }
}
