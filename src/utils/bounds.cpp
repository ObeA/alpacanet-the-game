#include "bounds.h"

Bounds::Bounds(glm::vec3 min, glm::vec3 max)
        : min(min), max(max), center() {
    update();
}

const glm::vec3& Bounds::getCenter() const {
    return center;
}

void Bounds::extend(const glm::vec3& point) {
    min.x = std::min(point.x, min.x);
    min.y = std::min(point.y, min.y);
    min.z = std::min(point.z, min.z);

    max.x = std::max(point.x, max.x);
    max.y = std::max(point.y, max.y);
    max.z = std::max(point.z, max.z);

    update();
}

void Bounds::update() {
    center = ((max - min) * 0.5f) + min;
}
