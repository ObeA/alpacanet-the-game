//
// Created by Leon on 4/30/2019.
//

#include "Material.h"

void Material::initialize() {
    createDescriptorSetLayout();
    createGraphicsPipeline();
}

void Material::cleanup() {
    vkDestroyDescriptorSetLayout(window->device, descriptorSetLayout, nullptr);
}

void Material::cleanupSwapChain() {
    vkDestroyPipeline(window->device, graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(window->device, pipelineLayout, nullptr);
}

VkShaderModule Material::createShaderModule(const std::vector<char>& code) {
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(window->device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
}