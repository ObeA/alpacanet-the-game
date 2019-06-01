#pragma once

#include "material.h"
#include "../graphics/graphics.h"

class BasicMaterial: public Material {
public:
    explicit BasicMaterial(Graphics* graphics, std::string shaderDirectory) : Material(graphics), shaderDirectory(shaderDirectory) {
    }

    void createDescriptorSet(VkDescriptorBufferInfo &uniformBufferInfo, VkDescriptorSet &descriptorSet) override;
protected:
    void createGraphicsPipeline() override;
    void createDescriptorSetLayout() override;
    std::string shaderDirectory;
};
