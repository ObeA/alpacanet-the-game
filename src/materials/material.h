#pragma once

#include "../window.h"
#include "../graphics/graphics.h"

class Material {
public:
    Material(Graphics* graphics) : graphics(graphics) { }

    virtual void initialize();
    virtual void cleanup();

    void cleanupSwapChain();

    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    virtual void createDescriptorSet(VkDescriptorBufferInfo &uniformBufferInfo, VkDescriptorSet &descriptorSet) = 0;

protected:
    Graphics* graphics;

    virtual void createGraphicsPipeline() = 0;
    virtual void createDescriptorSetLayout() = 0;

	void createBasicGraphicsPipeline(VkPipelineShaderStageCreateInfo shaderStages[]);
};
