#include "cube.h"
#include "../materials/material.h"
#include "../graphics/buffers/uniform_buffer.h"

std::vector<Vertex> Cube::getVertices() {
	return vertices;
}

std::vector<uint32_t> Cube::getIndices() {
	return indices;
}