#pragma once

#include <glm/glm.hpp>
#include <algorithm>

class Bounds {
public:
    explicit Bounds(glm::vec3 min = glm::vec3(std::numeric_limits<float>::max()), glm::vec3 max = glm::vec3(std::numeric_limits<float>::min()));
    explicit Bounds(glm::vec2 min, glm::vec2 max);
    Bounds(const Bounds &bounds);


    void extend(const glm::vec3& point);
    const glm::vec3& getCenter() const;
    const glm::vec3& getMin() const;
    const glm::vec3& getMax() const;

    float getDistance() const;

    Bounds getScaledCopy(glm::vec3 scale) const;
private:
    glm::vec3 min;
    glm::vec3 max;
    glm::vec3 center;

    void update();
};

