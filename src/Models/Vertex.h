//
// Created by Leon on 4/30/2019.
//

#ifndef ALPACAWEB_VERTEX_H
#define ALPACAWEB_VERTEX_H

#include "../Window.h"

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;
    glm::vec3 normal;

    static VkVertexInputBindingDescription getBindingDescription();

    static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions();
};


#endif //ALPACAWEB_VERTEX_H
