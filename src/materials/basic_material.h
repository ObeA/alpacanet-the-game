#pragma once

#include "material.h"

class BasicMaterial: public Material {
public:
    BasicMaterial(Window *window) : Material(window) {
    }

    void createDescriptorSet(VkDescriptorBufferInfo &uniformBufferInfo, VkDescriptorSet &descriptorSet);
protected:
    void createGraphicsPipeline();
    virtual void createDescriptorSetLayout();
};
