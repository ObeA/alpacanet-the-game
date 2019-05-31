#pragma once;

#include "drawable_object.h"
#include <random>

class ParticleSystem : public DrawableObject {
public:
    ParticleSystem(Game* game, Material* material, Material* shadowMaterial)
        : DrawableObject(game, material, shadowMaterial) {
    }
    ~ParticleSystem();

    void start() override;
    void createVertexBuffer() override;
    void draw(VkCommandBuffer cmdbuffer, size_t bufferOffset) override;
    void update() override;
    bool destroyFlag = false;
    float floorZ = 0;
    int amount = 0;
private:
    std::vector<Particle> particles;
    void setupParticles();
    void updateParticles();
    void initParticle(Particle * particle);
    void updateUniformBuffer(uint32_t currentImage, glm::mat4 view, glm::mat4 projection, glm::vec3 lightPos, glm::vec3 viewPos);
};