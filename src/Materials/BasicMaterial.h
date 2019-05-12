//
// Created by Leon on 4/30/2019.
//

#ifndef ALPACAWEB_BASICMATERIAL_H
#define ALPACAWEB_BASICMATERIAL_H

#include "Material.h"

class BasicMaterial: public Material {
public:
    BasicMaterial(Window *window) : Material(window) {
    }

    void createDescriptorSet(VkDescriptorBufferInfo &uniformBufferInfo, VkDescriptorSet &descriptorSet);
protected:
    void createGraphicsPipeline();
    virtual void createDescriptorSetLayout();
};


#endif //ALPACAWEB_BASICMATERIAL_H
