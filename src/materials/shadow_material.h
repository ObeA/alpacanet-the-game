#pragma once

#include "material.h"
#include "../game.h"

class ShadowMaterial : public Material {
public:
	ShadowMaterial(Graphics* graphics) : Material(graphics) {
	}

	void createDescriptorSet(VkDescriptorBufferInfo &uniformBufferInfo, VkDescriptorSet &descriptorSet);
protected:
	void createGraphicsPipeline();
	virtual void createDescriptorSetLayout();
};