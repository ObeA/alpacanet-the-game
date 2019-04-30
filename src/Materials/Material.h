//
// Created by Leon on 4/30/2019.
//

#ifndef ALPACAWEB_MATERIAL_H
#define ALPACAWEB_MATERIAL_H

#include "../Window.h"

class Window;

class Material {
public:
    Material(Window *window) : window(window) {
    }

    void initialize();

    void cleanup();

    void cleanupSwapChain();

    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

protected:
    Window* window;

    virtual void createGraphicsPipeline() = 0;

    VkShaderModule createShaderModule(const std::vector<char>& code);
    virtual void createDescriptorSetLayout() = 0;
};


#endif //ALPACAWEB_MATERIAL_H
