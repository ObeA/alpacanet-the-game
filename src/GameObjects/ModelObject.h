//
// Created by Leon on 5/4/2019.
//

#ifndef ALPACAWEB_MODELOBJECT_H
#define ALPACAWEB_MODELOBJECT_H

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

#include "GameObject.h"

class Window;
class ModelObject : public GameObject {
public:

    ModelObject(Window *window, Material *material, char* location) : GameObject(window, material), modelLocation(location) {
    }

    void generate(size_t swapchainImageSize);

    void updateUniformBuffer(uint32_t currentImage, glm::mat4 perspective, glm::vec3 lightPos, glm::mat4 depthMVP);

private:
    char* modelLocation;

    void loadModel();

    void createUniformBuffers(size_t swapChainImageSize);
};


#endif //ALPACAWEB_MODELOBJECT_H
