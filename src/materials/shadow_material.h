#pragma once

#include "material.h"

class ShadowMaterial : public Material {
public:
	ShadowMaterial(Window *window) : Material(window) {
	}

	void createDescriptorSet(VkDescriptorBufferInfo &uniformBufferInfo, VkDescriptorSet &descriptorSet);
protected:
	void createGraphicsPipeline();
	virtual void createDescriptorSetLayout();
};