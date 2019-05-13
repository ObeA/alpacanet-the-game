#pragma once

#include "../window.h"

class Material {
public:
    Material() = default;

    virtual void initialize();
    virtual void cleanup();

    void cleanupSwapChain();

    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    virtual void createDescriptorSet(VkDescriptorBufferInfo &uniformBufferInfo, VkDescriptorSet &descriptorSet) = 0;

protected:
    virtual void createGraphicsPipeline() = 0;

    VkShaderModule createShaderModule(const std::vector<char>& code);
    virtual void createDescriptorSetLayout() = 0;

	void createBasicGraphicsPipeline(VkPipelineShaderStageCreateInfo shaderStages[]);
};
