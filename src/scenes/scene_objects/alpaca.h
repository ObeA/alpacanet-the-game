#pragma once
#include "../../game_objects/model_object.h"
#include <algorithm>

class Alpaca: public ModelObject {
public:

    Alpaca(Game * game, Material * material, Material * shadowMaterial);

    void start() override;

    void moveTo(glm::vec2 position);

    void update();

    std::chrono::seconds nextMoveTime;

    int shear();

    int getWooliness();

private:
    glm::vec2 targetPosition;
    bool targetPositionReached = false;
    bool bounceCompleted = false;
    std::chrono::high_resolution_clock::time_point bounceStartTime;
    int wooliness;
};