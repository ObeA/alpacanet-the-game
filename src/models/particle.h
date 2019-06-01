#pragma once

#include "../window.h"

struct Particle {
    glm::vec4 pos;
    glm::vec4 color;
    float alpha;
    float size;

    glm::vec3 direction;

    static VkVertexInputBindingDescription getBindingDescription();

    static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions();
};