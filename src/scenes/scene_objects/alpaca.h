#pragma once

#include "../../game_objects/model_object.h"
#include <algorithm>

class Alpaca: public ModelObject {
public:
    Alpaca(Game * game, Material * material, Material * shadowMaterial);

    void start() override;

    void moveTo(glm::vec2 position);

    void update() override;

    unsigned long long nextMoveTick;

    int shear();

    int getWooliness() const;
    int getHappiness() const;

    bool hasReachedTargetPosition() const;

private:
    static constexpr float MIN_AGE = 0.5;
    static constexpr float MAX_AGE = 2;
    static constexpr float MAX_WOOL = 100;
    static constexpr glm::vec3 BASE_SIZE = glm::vec3(0.25, 0.25, 0.25);

    glm::vec2 targetPosition;
    bool targetPositionReached;
    bool bounceCompleted = false;
    float bouncyBoi;

    float happiness;
    float wooliness;
    float age;

    void updateAge();
    void updateWool();

    float getAgeScale() const;

    void updateHappiness();
};