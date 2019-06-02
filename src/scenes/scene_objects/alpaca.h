#pragma once

#include "../../game_objects/model_object.h"
#include <algorithm>

class Alpaca: public ModelObject {
public:
    Alpaca(Game * game, Material * material, Material * shadowMaterial);

    void moveTo(glm::vec2 position);

    void update() override;

    unsigned long long nextMoveTick;

    int shear();

    int getWooliness() const;
    int getHappiness() const;

    bool hasReachedTargetPosition() const;

protected:
    void postprocessMaterials(std::vector<tinyobj::material_t>& materials) override;

private:
    static constexpr float MIN_AGE = 0.5;
    static constexpr float MAX_AGE = 2;
    static constexpr glm::vec3 BASE_SIZE = glm::vec3(0.25, 0.25, 0.25);

    static constexpr std::array<glm::vec3, 4> COLORS = {
        glm::vec3(0.8, 0.02, 0.356),
        glm::vec3(0.7, 0.9, 0.59),
        glm::vec3(0.014, 0.641, 0.039),
        glm::vec3(0.396, 0.18, 0.1)
    };

    glm::vec2 targetPosition;
    bool targetPositionReached;
    bool bounceCompleted = false;
    float bouncyBoi;

    float happiness;
    float wooliness;
    float age;

    float getAgeScale() const;

    void updateAge();
    void updateWool();
    void updateHappiness();
    void updatePosition();
};