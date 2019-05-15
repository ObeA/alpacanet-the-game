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

#include "game_object.h"

class Window;

class ModelObject : public GameObject {
public:

    ModelObject(Game* game, Material* material, Material* shadowMaterial, char* location)
            : GameObject(game, material, shadowMaterial),
              modelLocation(location) {
        loadModel();
    }

    void generate(size_t swapchainImageSize);

    void updateUniformBuffer(uint32_t currentImage, glm::mat4 perspective, glm::vec3 lightPos);

private:
    char* modelLocation;

    void loadModel();

    void createUniformBuffers(size_t swapChainImageSize);
};