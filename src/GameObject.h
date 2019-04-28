//
// Created by Leon on 4/28/2019.
//

#ifndef ALPACAWEB_GAMEOBJECT_H
#define ALPACAWEB_GAMEOBJECT_H

#include "Window.h"

class Window;

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription();

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();
};

struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};


class GameObject {
public:
    GameObject(Window *window) : window(window) {
    }

//    virtual void generate(size_t swapchainImageSize);
//
//    virtual void updateUniformBuffer(uint32_t currentImage, glm::mat4 perspective) = 0;
//
//    virtual void cleanup(size_t swapchainImages) = 0;
//
//    virtual void draw(VkCommandBuffer cmdbuffer, VkPipelineLayout pipelineLayout, size_t bufferOffset) = 0;

    Window *window;
};


#endif //ALPACAWEB_GAMEOBJECT_H
