#pragma once

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <array>
#include <vector>
#include <chrono>
#include <algorithm>
#include <cstring>

#include "drawable_object.h"
#include "../models/vertex.h"

class Cube : public DrawableObject {
public:

    Cube(Game* game, Material* material, Material* shadowMaterial) : DrawableObject(game, material, shadowMaterial) {
    }

    std::vector<Vertex> getVertices();

    std::vector<uint32_t> getIndices();

    const std::vector<Vertex> vertices = {
            {{-0.5f, -0.5f, 0.5f},  {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f,  -0.5f, 0.5f},  {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
            {{0.5f,  0.5f,  0.5f},  {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
            {{-0.5f, 0.5f,  0.5f},  {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

            {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f,  -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
            {{0.5f,  0.5f,  -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
            {{-0.5f, 0.5f,  -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
    };

    const std::vector<uint32_t> indices = {
            0, 1, 3, 3, 1, 2,
            1, 5, 2, 2, 5, 6,
            5, 4, 6, 6, 4, 7,
            4, 0, 7, 7, 0, 3,
            3, 2, 7, 7, 2, 6,
            4, 5, 0, 0, 5, 1
    };
};
