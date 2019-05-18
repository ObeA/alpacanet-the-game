#include "model_object.h"
#include "../graphics/buffers/uniform_buffer.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

void ModelObject::loadModel() {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelLocation)) {
        throw std::runtime_error(warn + err);
    }

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            Vertex vertex = {};

            vertex.pos = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
            };

            if(index.texcoord_index != -1) {
                vertex.texCoord = {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };
            }

            if(index.normal_index != -1) {
                vertex.normal = {
                        attrib.normals[3 * index.normal_index + 0],
                        attrib.normals[3 * index.normal_index + 1],
                        attrib.normals[3 * index.normal_index + 2]
                };
            }

            vertex.color = {1.0f, 1.0f, 1.0f};

            vertices.push_back(vertex);
            indices.push_back(indices.size());
        }
    }
}

void ModelObject::updateUniformBuffer(uint32_t currentImage, glm::mat4 view, glm::mat4 projection, glm::vec3 lightPos) {
//    position.x = glm::clamp(position.x + (rand() % 2 - .5f) / 500, -3.0f, 3.0f);
//    position.y = glm::clamp(position.y + (rand() % 2 - .5f) / 500, -3.0f, 3.0f);
//    position.z = glm::clamp(position.z + (rand() % 2 - .5f) / 500, -3.0f, 3.0f);

    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo = {};
    ubo.model = glm::mat4(1.0f);
    ubo.model = glm::translate(ubo.model, position);//position
    ubo.model = glm::rotate(ubo.model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));//rotation
    ubo.model = glm::scale(ubo.model, scale);

    ubo.view = view;
    ubo.projection = projection;
    ubo.projection[1][1] *= -1;

	//TODO: change when rendered from light
    ubo.lightPos = lightPos;
    ubo.depthBiasMVP = ubo.view * ubo.projection;

    auto device = game->getGraphics()->getLogicalDevice()->getDevice();
    void* data;
    vkMapMemory(device, uniformBuffers[currentImage]->getMemory(), 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(device, uniformBuffers[currentImage]->getMemory());

	UniformBufferObjectOffscreen uboOffscreen = {};
	uboOffscreen.model = ubo.model;
	uboOffscreen.depthVP = ubo.view * ubo.projection;

	void* offscreenData;
	vkMapMemory(device, offscreenUniformBuffer->getMemory(), 0, sizeof(uboOffscreen), 0, &offscreenData);
	memcpy(offscreenData, &uboOffscreen, sizeof(uboOffscreen));
	vkUnmapMemory(device, offscreenUniformBuffer->getMemory());
}

void ModelObject::createUniformBuffers(size_t swapChainImageSize) {
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);
    uniformBuffers.resize(swapChainImageSize);

    auto device = game->getGraphics()->getLogicalDevice();
    for (size_t i = 0; i < swapChainImageSize; i++) {
        uniformBuffers[i] = new UniformBuffer(device, bufferSize);
    }

	VkDeviceSize offscreenBufferSize = sizeof(UniformBufferObjectOffscreen);
    offscreenUniformBuffer = new UniformBuffer(device, offscreenBufferSize);
}

void ModelObject::start() {
    loadModel();

    GameObject::start();
}
