#pragma once

#include "drawable_object.h"
#include <random>

class ParticleSystem : public DrawableObject {
public:
    ParticleSystem(Game* game, Material* material, Material* shadowMaterial)
        : DrawableObject(game, material, shadowMaterial) {
    }

    void start() override;
    void createVertexBuffer() override;
    void draw(VkCommandBuffer cmdbuffer, size_t bufferOffset) override;
    void update() override;
    bool destroyFlag = false;
    float floorZ = 0.1;
    int amount = 0;
private:
    std::vector<Particle> particles;
    void setupParticles();
    void updateParticles();
    void initParticle(Particle * particle);
};