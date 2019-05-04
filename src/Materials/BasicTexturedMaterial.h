//
// Created by Leon on 5/4/2019.
//

#ifndef ALPACAWEB_BASICTEXTUREDMATERIAL_H
#define ALPACAWEB_BASICTEXTUREDMATERIAL_H

#include "material.h"

class BasicTexturedMaterial: public Material {
public:
    BasicTexturedMaterial(Window *window) : Material(window) {
    }

    void createDescriptorSet(VkDescriptorBufferInfo &uniformBufferInfo, VkDescriptorSet &descriptorSet);
protected:
    void createGraphicsPipeline();
    virtual void createDescriptorSetLayout();
};


#endif //ALPACAWEB_BASICTEXTUREDMATERIAL_H
