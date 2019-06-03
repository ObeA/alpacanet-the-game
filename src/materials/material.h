#pragma once

#include "../window.h"
#include "../graphics/graphics.h"

class Material {
public:
    explicit Material(Graphics* graphics, std::string shaderDirectory) : graphics(graphics), shaderDirectory(shaderDirectory) { }
    virtual ~Material();

    virtual void initialize();

    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    virtual void createDescriptorSet(VkDescriptorBufferInfo &uniformBufferInfo, VkDescriptorSet &descriptorSet) = 0;

protected:
    Graphics* graphics;

    virtual void createGraphicsPipeline() = 0;
    virtual void createDescriptorSetLayout() = 0;

	virtual void createBasicGraphicsPipeline(VkPipelineShaderStageCreateInfo shaderStages[]);
    std::string shaderDirectory;
};
