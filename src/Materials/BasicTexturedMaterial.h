//
// Created by Leon on 5/4/2019.
//

#ifndef ALPACAWEB_BASICTEXTUREDMATERIAL_H
#define ALPACAWEB_BASICTEXTUREDMATERIAL_H

#include "Material.h"

class BasicTexturedMaterial: public Material {
public:
    BasicTexturedMaterial(Window *window, char* texture) : Material(window), textureLocation(texture) {
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


#endif //ALPACAWEB_BASICTEXTUREDMATERIAL_H
