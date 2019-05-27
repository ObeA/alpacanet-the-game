#pragma once

#include "../window.h"

struct Particle {
    glm::vec4 pos;
    glm::vec4 color;
    float alpha;
    float size;
    float rotation;
    glm::vec4 vel;
    float rotationSpeed;

    static VkVertexInputBindingDescription getBindingDescription();

    static std::array<VkVertexInputAttributeDescription, 5> getAttributeDescriptions();
};