#pragma once

#include "material.h"
#include "../graphics/graphics.h"

class BasicMaterial: public Material {
public:
    explicit BasicMaterial(Graphics* graphics) : Material(graphics) {
    }

    void createDescriptorSet(VkDescriptorBufferInfo &uniformBufferInfo, VkDescriptorSet &descriptorSet) override;
protected:
    void createGraphicsPipeline() override;
    void createDescriptorSetLayout() override;
};
