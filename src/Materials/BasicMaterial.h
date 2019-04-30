//
// Created by Leon on 4/30/2019.
//

#ifndef ALPACAWEB_BASICMATERIAL_H
#define ALPACAWEB_BASICMATERIAL_H

#include "material.h"

class BasicMaterial: public Material {
public:
    BasicMaterial(Window *window) : Material(window) {
    }
protected:
    void createGraphicsPipeline();
    virtual void createDescriptorSetLayout();
};


#endif //ALPACAWEB_BASICMATERIAL_H
