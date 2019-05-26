#include "particlesystem.h"

ParticleSystem::~ParticleSystem() {
    vertexBuffer->unmap();
}

void ParticleSystem::setupParticles()
{
    for (size_t i = 0; i < 100; i++)
    {
        Particle p;
        initParticle(&p);
        particles.push_back(p);
    }
}

void ParticleSystem::updateParticles()
{
    for (auto& particle : particles)
    {
        particle.pos.x = (std::rand() % 50) * .1f;
        particle.pos.y = (std::rand() % 50) * .1f;
        particle.pos.z = (std::rand() % 50) * .1f;

        particle.pos += glm::vec4(position, 0.0f);
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
    particle->vel = glm::vec4(0.0f, 3.5f, 0.0f, 0.0f);
    particle->alpha = 1;
    particle->size = 1;
    float r = (std::rand() % 100) * .01f;
    float g = (std::rand() % 100) * .01f;
    float b = (std::rand() % 100) * .01f;
    particle->color = glm::vec4(r, g, b, 1.0f);
    particle->rotation = 1;
    particle->rotationSpeed = 1;

    particle->pos.x = std::rand() % 5;
    particle->pos.y = std::rand() % 5;
    particle->pos.z = std::rand() % 5;

    particle->pos += glm::vec4(position, 0.0f);
}

void ParticleSystem::updateUniformBuffer(uint32_t currentImage, glm::mat4 view, glm::mat4 projection, glm::vec3 lightPos, glm::vec3 viewPos)
{
    updateParticles();
    DrawableObject::updateUniformBuffer(currentImage, view, projection , lightPos, viewPos);
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