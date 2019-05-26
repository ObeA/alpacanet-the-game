#include "model_object.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

void ModelObject::loadModel() {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelLocation.c_str(), "assets/models/")) {
        throw std::runtime_error(warn + err);
    }

    for (const auto& shape : shapes) {
        glm::vec3 color = glm::vec3(0);
        if (shape.mesh.material_ids[0] != -1) {
            auto matchedMaterial = materials[shape.mesh.material_ids[0]];
            color = { matchedMaterial.diffuse[0], matchedMaterial.diffuse[1], matchedMaterial.diffuse[2] };
        }

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

            if (color == glm::vec3(0)) {
                vertex.color = { 1.0f, 1.0f, 1.0f };
            }
            else {
                vertex.color = color;
            }

            vertices.push_back(vertex);
            indices.push_back(indices.size());
        }
    }
}

void ModelObject::start() {
    loadModel();

    DrawableObject::start();
}
