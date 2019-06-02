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

    postprocessMaterials(materials);

    bool useDefaultMaterial = materials.empty();
    for (const auto& shape : shapes) {
        int faceId = 0;
        tinyobj::material_t faceMaterial;
        int verticesInFace = std::numeric_limits<int>::max();
        int currentFaceVertexIndex = 0;

        if (!useDefaultMaterial) {
            faceMaterial = materials[shape.mesh.material_ids[faceId]];
            verticesInFace = shape.mesh.num_face_vertices[faceId];
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

            if (useDefaultMaterial) {
                vertex.color = glm::vec3(1.0f);
            } else {
                vertex.color = glm::vec3(faceMaterial.diffuse[0], faceMaterial.diffuse[1], faceMaterial.diffuse[2]);
            }

            vertices.push_back(vertex);
            indices.push_back(indices.size());

            currentFaceVertexIndex++;
            if (currentFaceVertexIndex > verticesInFace - 1 && faceId + 1 < shape.mesh.num_face_vertices.size()) {
                currentFaceVertexIndex = 0;
                verticesInFace = shape.mesh.num_face_vertices[++faceId];
                faceMaterial = materials[shape.mesh.material_ids[faceId]];
            }
        }
    }
}

void ModelObject::start() {
    loadModel();

    DrawableObject::start();
}
