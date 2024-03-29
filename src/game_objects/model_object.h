#include <utility>

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
#include <tiny_obj_loader.h>

#include "drawable_object.h"

class ModelObject : public DrawableObject {
public:
    ModelObject(Game* game, Material* material, Material* shadowMaterial, std::string location)
            : DrawableObject(game, material, shadowMaterial),
              modelLocation(std::move(location)) {
    }

    void start() override;
    void update() override { }

protected:
    virtual void postprocessMaterials(std::vector<tinyobj::material_t>& materials) { }

private:
    const std::string modelLocation;

    void loadModel();
};