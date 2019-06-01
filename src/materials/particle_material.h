#pragma once

#include "basic_material.h"

class ParticleMaterial : public BasicMaterial {
public:
    explicit ParticleMaterial(Graphics* graphics, std::string shaderDirectory) : BasicMaterial(graphics, shaderDirectory) {
    }
protected:
    void createBasicGraphicsPipeline(VkPipelineShaderStageCreateInfo shaderStages[]) override;
};