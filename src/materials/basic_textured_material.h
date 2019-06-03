#pragma once

#include "material.h"

class BasicTexturedMaterial: public Material {
public:
    BasicTexturedMaterial(Graphics* graphics, char* texture, std::string shaderDirectory) : Material(graphics, shaderDirectory), textureLocation(texture) {
    }
    ~BasicTexturedMaterial() override;

	void initialize() override;

    void createDescriptorSet(VkDescriptorBufferInfo &uniformBufferInfo, VkDescriptorSet &descriptorSet) override;
protected:
	char* textureLocation;
	VkImageView textureImageView;
	VkSampler textureSampler;
	Image* textureImage;

    void createGraphicsPipeline() override;
    void createDescriptorSetLayout() override;
	void createTextureImage();
	void createTextureImageView();
	void createTextureSampler();
};