#pragma once

#include "imgui/imgui.h"
#include "../renderer.h"
#include <string>

class Alpaca;

class GUI {
public:
    GUI(Renderer* renderer, LogicalDevice* device, Window* window);
    ~GUI();
    void init(float width, float height);
    void initResources(VkRenderPass renderPass, VkQueue copyQueue);
    void newFrame();
    void updateBuffers();
    void drawFrame(VkCommandBuffer commandBuffer);
    void setScene(Scene* newScene);
private:
    VkSampler sampler;
    Buffer* vertexBuffer =  nullptr;
    Buffer* indexBuffer = nullptr;
    int32_t vertexCount = 0;
    int32_t indexCount = 0;
    Image* fontImage;
    VkImageView fontView = VK_NULL_HANDLE;
    VkPipelineCache pipelineCache;
    VkPipelineLayout pipelineLayout;
    VkPipeline pipeline;
    VkDescriptorPool descriptorPool;
    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorSet descriptorSet;
    Renderer* renderer;
    LogicalDevice* device;
    Window* window;
    Scene* scene = nullptr;
    void onMouseMove(double x, double y);
    void onMouseButton(int key, int scancode, int mods);
    glm::vec2 currentMousePosition = glm::vec2(0);
    std::vector<bool> mouseButtons = {false, false};
};