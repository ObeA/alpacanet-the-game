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

    int getWooliness();

private:
    const float MIN_AGE = 0.5;
    const float MAX_AGE = 2;
    const glm::vec3 BASE_SIZE = glm::vec3(0.25, 0.25, 0.25);

    glm::vec2 targetPosition;
    bool targetPositionReached = false;
    bool bounceCompleted = false;
    float bouncyBoi;
    int wooliness;
    float age;

    void updateAge();
};