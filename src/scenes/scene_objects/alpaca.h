#pragma once
#include "../../game_objects/model_object.h"
#include <algorithm>

class Alpaca: public ModelObject {
public:

    Alpaca(Game * game, Material * material, Material * shadowMaterial);

    void start() override;

    void moveTo(glm::vec2 position);

    virtual void updateUniformBuffer(uint32_t currentImage, glm::mat4 view, glm::mat4 projection, glm::vec3 lightPos, glm::vec3 viewPos) override;

    void update();

    std::chrono::seconds nextMoveTime;

    int shear();

private:
    glm::vec2 targetPosition;
    bool targetPositionReached = false;
    bool bounceCompleted = false;
    std::chrono::steady_clock::time_point bounceStartTime;
    int wooliness;
};