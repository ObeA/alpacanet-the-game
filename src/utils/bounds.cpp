#include "bounds.h"

Bounds::Bounds(glm::vec3 min, glm::vec3 max)
        : min(min), max(max), center() {
    update();
}

Bounds::Bounds(const Bounds &bounds)
    : min(bounds.min), max(bounds.max), center() {
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

Bounds& Bounds::getScaledCopy(glm::vec3 scale) {
    auto copy = Bounds(*this);
    auto normalizedMin = copy.min - copy.center;
    auto normalizedMax = copy.max - copy.center;
    normalizedMin *= scale;
    normalizedMax *= scale;
    //copy.min = normalizedMin + copy.center;
    copy.max = normalizedMax + copy.center;

    copy.update();
    return copy;
}
