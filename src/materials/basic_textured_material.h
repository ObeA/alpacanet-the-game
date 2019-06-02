#pragma once

#include "material.h"

class BasicTexturedMaterial: public Material {
public:
    BasicTexturedMaterial(Graphics* graphics, char* texture, std::string shaderDirectory) : Material(graphics, shaderDirectory), textureLocation(texture) {
    }

	void initialize();

    void createDescriptorSet(VkDescriptorBufferInfo &uniformBufferInfo, VkDescriptorSet &descriptorSet);

	void cleanup();
protected:
	char* textureLocation;
	VkImageView textureImageView;
	VkSampler textureSampler;
	Image* textureImage;

    void createGraphicsPipeline();
    virtual void createDescriptorSetLayout();
	void createTextureImage();
	void createTextureImageView();
	void createTextureSampler();
};