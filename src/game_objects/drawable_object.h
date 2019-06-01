#pragma once

#include "game_object.h"
#include "../materials/material.h"

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 lightSpace;
    glm::vec3 lightPos;
};

class DrawableObject : public GameObject {
public:
    DrawableObject(Game* game, Material* material, Material* shadowMaterial);
    ~DrawableObject();

    void start();

    std::vector<VkDescriptorSet> descriptorSets{};
    std::vector<VkDescriptorSet> offscreenDescriptorSets{};

    virtual void draw(VkCommandBuffer cmdbuffer, size_t bufferOffset);

    virtual std::vector<Vertex> getVertices();

    virtual std::vector<uint32_t> getIndices();

    virtual void updateUniformBuffer(uint32_t currentImage, Camera* camera, Light* light);

    Material *material;
    Material *shadowMaterial;
protected:
    Buffer* vertexBuffer;
    Buffer* indexBuffer;

    std::vector<Buffer*> uniformBuffers;
    std::vector<Buffer*> offscreenUniformBuffers;

    virtual void createVertexBuffer();

    virtual void createIndexBuffer();

    void createDescriptorSet(size_t swapChainImageSize);

    void createUniformBuffers(size_t swapChainImageSize);

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};