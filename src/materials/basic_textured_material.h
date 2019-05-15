#pragma once

#include "material.h"

class BasicTexturedMaterial: public Material {
public:
    BasicTexturedMaterial(Graphics* graphics, char* texture) : Material(graphics), textureLocation(texture) {
    }

	void initialize();

    void createDescriptorSet(VkDescriptorBufferInfo &uniformBufferInfo, VkDescriptorSet &descriptorSet);

	void cleanup();
protected:
	char* textureLocation;
	VkImageView textureImageView;
	VkSampler textureSampler;
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;

    void createGraphicsPipeline();
    virtual void createDescriptorSetLayout();
	void createTextureImage();
	void createTextureImageView();
	void createTextureSampler();
};