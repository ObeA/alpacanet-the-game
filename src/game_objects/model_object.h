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

class ModelObject : public DrawableObject {
public:

    ModelObject(Game* game, Material* material, Material* shadowMaterial, char* location)
            : DrawableObject(game, material, shadowMaterial),
              modelLocation(location) {
    }

    void start() override;

private:
    char* modelLocation;

    void loadModel();
};