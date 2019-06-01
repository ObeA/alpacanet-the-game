#include "particlesystem.h"

ParticleSystem::~ParticleSystem() {
    vertexBuffer->unmap();
}

void ParticleSystem::setupParticles()
{
    for (size_t i = 0; i < amount; i++)
    {
        Particle p;
        initParticle(&p);
        particles.push_back(p);
    }
}

float moveTowards(float from, float to, float amount) {
    float result = 0;
    if (from < to)
        result = std::min(from + amount, to);
    else if (from > to)
        result = std::max(from - amount, to);
    else
        result = to;
    return result;
}

void ParticleSystem::updateParticles()
{
    bool allReady = true;
    for (auto& particle : particles)
    {
        if (particle.pos.z >= -position.z + floorZ) {
            particle.pos.x -= particle.direction.x / 10;
            particle.pos.y -= particle.direction.y / 10;
            particle.pos.z -= particle.direction.z / 10;
            particle.direction.x = moveTowards(particle.direction.x, 0.0f, abs(particle.direction.x) / 100);
            particle.direction.y = moveTowards(particle.direction.y, 0.0f, abs(particle.direction.y) / 100);
            particle.direction.z = moveTowards(particle.direction.z, 1.0f, 0.01f);
        }
        else {
            particle.alpha = moveTowards(particle.alpha, 0, 0.002f);
        }
        if (particle.alpha > 0) {
            allReady = false;
        }
    }
    if (allReady) {
        destroyFlag = true;
    }

    size_t size = particles.size() * sizeof(Particle);

    VkDeviceSize bufferSize = particles.size() * sizeof(Particle);

    Buffer stagingBuffer(
        game->getGraphics()->getLogicalDevice(),
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        particles.data()
    );

    stagingBuffer.copyTo(vertexBuffer);
}

void ParticleSystem::initParticle(Particle *particle)
{
    glm::vec2 direction2 = glm::normalize(glm::vec2(((std::rand() % 100 - 50)) * .01f, ((std::rand() % 100 - 50)) * .01f));
    direction2 *= ((std::rand() % 50) + 50) * 0.01f;
    auto directionZ = -(std::rand() % 50) * 0.01f;
    particle->direction = glm::vec3(direction2.x, direction2.y, directionZ);
    particle->alpha = 1;
    particle->size = 1.5f;
    float r = (std::rand() % 100) * .01f;
    float g = (std::rand() % 100) * .01f;
    float b = (std::rand() % 100) * .01f;
    particle->color = glm::vec4(r, g, b, 1.0f);
    particle->pos = glm::vec4(0);

    //particle->pos += glm::vec4(position, 0.0f);
}

void ParticleSystem::start() {
    setupParticles();

    createVertexBuffer();

    auto numberOfSwapchainImages = game->getGraphics()->getRenderer()->getSwapchain()->getImages().size();
    createUniformBuffers(numberOfSwapchainImages);
    createDescriptorSet(numberOfSwapchainImages);
}

void ParticleSystem::createVertexBuffer() {
    VkDeviceSize bufferSize = particles.size() * sizeof(Particle);

    Buffer stagingBuffer(
        game->getGraphics()->getLogicalDevice(),
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        particles.data()
    );

    vertexBuffer = new Buffer(
        game->getGraphics()->getLogicalDevice(),
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );

    stagingBuffer.copyTo(vertexBuffer);
}

void ParticleSystem::draw(VkCommandBuffer cmdbuffer, size_t bufferOffset) {
    VkBuffer vertexBuffers[] = { vertexBuffer->getBuffer() };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(cmdbuffer, 0, 1, vertexBuffers, offsets);

    vkCmdDraw(cmdbuffer, static_cast<uint32_t>(particles.size()), 1, 0, 0);
}

void ParticleSystem::update() {
    updateParticles();
}