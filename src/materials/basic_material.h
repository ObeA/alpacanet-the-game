#pragma once

#include "material.h"

class BasicMaterial: public Material {
public:
    explicit BasicMaterial() : Material() {
    }

    void createDescriptorSet(VkDescriptorBufferInfo &uniformBufferInfo, VkDescriptorSet &descriptorSet) override;
protected:
    void createGraphicsPipeline() override;
    void createDescriptorSetLayout() override;
};
