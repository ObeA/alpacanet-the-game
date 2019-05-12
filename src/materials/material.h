#pragma once

#include "../window.h"

class Window;

class Material {
public:
    Material(Window *window) : window(window) {
    }

    virtual void initialize();

    virtual void cleanup();

    void cleanupSwapChain();

    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    virtual void createDescriptorSet(VkDescriptorBufferInfo &uniformBufferInfo, VkDescriptorSet &descriptorSet) = 0;

protected:
    Window* window;

    virtual void createGraphicsPipeline() = 0;

    VkShaderModule createShaderModule(const std::vector<char>& code);
    virtual void createDescriptorSetLayout() = 0;

	void createBasicGraphicsPipeline(VkPipelineShaderStageCreateInfo shaderStages[]);
};
