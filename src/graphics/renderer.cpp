#include "renderer.h"
#include "../scenes/scene.h"
#include "../game_objects/drawable_object.h"
#include "vulkan_utilities.h"
#include "../managers/material_manager.h"
#include "gui/gui.h"

Renderer::Renderer(Window* window, Surface* surface, LogicalDevice* logicalDevice)
        : window(window),
          surface(surface),
          logicalDevice(logicalDevice),
          swapchain(logicalDevice,
                    surface,
                    surface->hasLimitedExtents()
                    ? surface->getCapabilities().currentExtent
                    : window->getExtents()),
          scene(nullptr) {
    createImageViews();
    createDescriptorPool();

    initializeOffscreenRendering();

    initializeRenderPass();
    createDepthResources();
    createFramebuffers();

    createSyncObjects();
    createGUI();
}

Renderer::~Renderer() {
    auto device = logicalDevice->getDevice();

    vkFreeCommandBuffers(device, logicalDevice->getCommandPool(), static_cast<uint32_t>(commandBuffers.size()),
                         commandBuffers.data());
    vkDestroyDescriptorPool(device, descriptorPool, nullptr);

    //vkDestroySampler(device, offscreenDepthSampler, nullptr);
    //vkDestroyImageView(device, offscreenDepthImageView, nullptr);
    //vkDestroyFramebuffer(device, offscreenFrameBuffer, nullptr);
    //vkDestroyRenderPass(device, offscreenRenderPass, nullptr);

    vkDestroyImageView(device, depthImageView, nullptr);

    for (auto& swapChainFramebuffer : swapChainFramebuffers) {
        vkDestroyFramebuffer(device, swapChainFramebuffer, nullptr);
    }

    vkDestroyRenderPass(device, renderPass, nullptr);

    for (auto& swapChainImageView : swapChainImageViews) {
        vkDestroyImageView(device, swapChainImageView, nullptr);
    }

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(device, inFlightFences[i], nullptr);
    }
}

void Renderer::recreateCommandBuffer() {
    vkQueueWaitIdle(logicalDevice->getGraphicsQueue());
    vkFreeCommandBuffers(logicalDevice->getDevice(), logicalDevice->getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
    createCommandbuffers();
}

void Renderer::initializeRenderPass() {
    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = surface->getFormat().format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription depthAttachment = {};
    depthAttachment.format = findDepthFormat();
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef = {};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 2> attachments = {colorAttachment, depthAttachment};
    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(logicalDevice->getDevice(), &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
}

void Renderer::initializeOffscreenRenderPass() {
    VkAttachmentDescription attachmentDescription = {};
    attachmentDescription.format = findDepthFormat();
    attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
    attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

    VkAttachmentReference depthReference = {};
    depthReference.attachment = 0;
    depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 0;
    subpass.pDepthStencilAttachment = &depthReference;

    std::array<VkSubpassDependency, 2> dependencies{};
    dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[0].dstSubpass = 0;
    dependencies[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    dependencies[0].dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependencies[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
    dependencies[0].dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    dependencies[1].srcSubpass = 0;
    dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[1].srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    dependencies[1].srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    VkRenderPassCreateInfo renderPassCreateInfo = {};
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.attachmentCount = 1;
    renderPassCreateInfo.pAttachments = &attachmentDescription;
    renderPassCreateInfo.subpassCount = 1;
    renderPassCreateInfo.pSubpasses = &subpass;
    renderPassCreateInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
    renderPassCreateInfo.pDependencies = dependencies.data();

    VK_CHECK_RESULT(
            vkCreateRenderPass(logicalDevice->getDevice(), &renderPassCreateInfo, nullptr, &offscreenRenderPass))
}

void Renderer::initializeOffscreenFramebuffer() {
    VkImageCreateInfo imageCreateInfo = {};
    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.extent.height = imageCreateInfo.extent.width = SHADOWMAP_DIMENSION;
    imageCreateInfo.arrayLayers = imageCreateInfo.mipLevels = imageCreateInfo.extent.depth = 1;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageCreateInfo.format = findDepthFormat();
    imageCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    VK_CHECK_RESULT(vkCreateImage(logicalDevice->getDevice(), &imageCreateInfo, nullptr, &offscreenDepthImage))

    VkMemoryAllocateInfo memoryAllocateInfo = {};
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    VkMemoryRequirements memoryRequirements;
    vkGetImageMemoryRequirements(logicalDevice->getDevice(), offscreenDepthImage, &memoryRequirements);
    memoryAllocateInfo.allocationSize = memoryRequirements.size;
    memoryAllocateInfo.memoryTypeIndex = logicalDevice->getPhysicalDevice()->findMemoryType(
            memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    VK_CHECK_RESULT(
            vkAllocateMemory(logicalDevice->getDevice(), &memoryAllocateInfo, nullptr, &offscreenDepthImageMemory))
    VK_CHECK_RESULT(vkBindImageMemory(logicalDevice->getDevice(), offscreenDepthImage, offscreenDepthImageMemory, 0))

    VkImageViewCreateInfo depthStencilView = {};
    depthStencilView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    depthStencilView.viewType = VK_IMAGE_VIEW_TYPE_2D;
    depthStencilView.format = findDepthFormat();
    depthStencilView.subresourceRange = {};
    depthStencilView.subresourceRange.baseMipLevel = 0;
    depthStencilView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    depthStencilView.subresourceRange.levelCount = 1;
    depthStencilView.subresourceRange.baseArrayLayer = 0;
    depthStencilView.subresourceRange.layerCount = 1;
    depthStencilView.image = offscreenDepthImage;
    VK_CHECK_RESULT(vkCreateImageView(logicalDevice->getDevice(), &depthStencilView, nullptr, &offscreenDepthImageView))

    VkSamplerCreateInfo samplerCreateInfo = {};
    samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerCreateInfo.magFilter = SHADOWMAP_FILTER;
    samplerCreateInfo.minFilter = SHADOWMAP_FILTER;
    samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerCreateInfo.addressModeW = samplerCreateInfo.addressModeV = samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCreateInfo.mipLodBias = 0.0f;
    samplerCreateInfo.maxAnisotropy = 1.0f;
    samplerCreateInfo.minLod = 0.0f;
    samplerCreateInfo.maxLod = 1.0f;
    samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
    VK_CHECK_RESULT(vkCreateSampler(logicalDevice->getDevice(), &samplerCreateInfo, nullptr, &offscreenDepthSampler))

    VkFramebufferCreateInfo bufferCreateInfo = {};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.renderPass = offscreenRenderPass;
    bufferCreateInfo.attachmentCount = 1;
    bufferCreateInfo.pAttachments = &offscreenDepthImageView;
    bufferCreateInfo.height = bufferCreateInfo.width = SHADOWMAP_DIMENSION;
    bufferCreateInfo.layers = 1;
    VK_CHECK_RESULT(vkCreateFramebuffer(logicalDevice->getDevice(), &bufferCreateInfo, nullptr, &offscreenFrameBuffer))
}

void Renderer::initializeOffscreenRendering() {
    initializeOffscreenRenderPass();
    initializeOffscreenFramebuffer();
}

VkFormat Renderer::findDepthFormat() {
    return findSupportedFormat(
            {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

VkFormat Renderer::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling,
                                       VkFormatFeatureFlags features) {
    for (VkFormat format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(logicalDevice->getPhysicalDevice()->getDevice(), format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }

    throw std::runtime_error("failed to find supported format!");
}

void Renderer::createDescriptorPool() {
    const size_t objectAmount = 100;
    std::array<VkDescriptorPoolSize, objectAmount * 2> poolSizes = {};

    auto numberOfSwapchainImages = swapchain.getImages().size();
    for (size_t i = 0; i < objectAmount; i++) {
        poolSizes[i].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[i].descriptorCount = static_cast<uint32_t>(numberOfSwapchainImages);
        poolSizes[i + objectAmount].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[i + objectAmount].descriptorCount = static_cast<uint32_t>(numberOfSwapchainImages);
    }

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(numberOfSwapchainImages * objectAmount);

    VK_CHECK_RESULT(vkCreateDescriptorPool(logicalDevice->getDevice(), &poolInfo, nullptr, &descriptorPool))
}

void Renderer::createFramebuffers() {
    swapChainFramebuffers.resize(swapChainImageViews.size());

    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
        std::array<VkImageView, 2> attachments = {
                swapChainImageViews[i],
                depthImageView
        };

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = swapchain.getExtents().width;
        framebufferInfo.height = swapchain.getExtents().height;
        framebufferInfo.layers = 1;

        VK_CHECK_RESULT(vkCreateFramebuffer(logicalDevice->getDevice(), &framebufferInfo, nullptr, &swapChainFramebuffers[i]))
    }
}

void Renderer::createImageViews() {
    auto swapChainImages = swapchain.getImages();
    swapChainImageViews.resize(swapChainImages.size());

    for (uint32_t i = 0; i < swapChainImages.size(); i++) {
        swapChainImageViews[i] = logicalDevice->createImageView(swapChainImages[i],
                                                                surface->getFormat().format,
                                                                VK_IMAGE_ASPECT_COLOR_BIT);
    }
}

void Renderer::createDepthResources() {
    VkFormat depthFormat = findDepthFormat();

    depthImage = new Image(logicalDevice, swapchain.getExtents(), depthFormat,
                           VK_IMAGE_TILING_OPTIMAL,
                           VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    depthImageView = logicalDevice->createImageView(depthImage->getImage(), depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
    depthImage->transitionLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
}

void Renderer::createCommandbuffers() {
    commandBuffers.resize(swapChainFramebuffers.size());

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = logicalDevice->getCommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

    if (vkAllocateCommandBuffers(logicalDevice->getDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }

    for (size_t i = 0; i < commandBuffers.size(); i++) {
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        beginInfo.pInheritanceInfo = nullptr; // Optional

        if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        std::array<VkClearValue, 2> clearValues = {};
        {
            clearValues[0].depthStencil = {1.0f, 0};

            VkRenderPassBeginInfo renderPassBeginInfo = {};
            renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassBeginInfo.renderPass = offscreenRenderPass;
            renderPassBeginInfo.framebuffer = offscreenFrameBuffer;
            renderPassBeginInfo.renderArea.extent.width = renderPassBeginInfo.renderArea.extent.height = SHADOWMAP_DIMENSION;
            renderPassBeginInfo.clearValueCount = 1;
            renderPassBeginInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

            VkViewport viewport = {};
            viewport.width = SHADOWMAP_DIMENSION;
            viewport.height = SHADOWMAP_DIMENSION;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
            vkCmdSetViewport(commandBuffers[i], 0, 1, &viewport);

            VkRect2D scissor = {};
            scissor.extent.height = scissor.extent.width = SHADOWMAP_DIMENSION;
            vkCmdSetScissor(commandBuffers[i], 0, 1, &scissor);

            // Set depth bias (aka "Polygon offset")
            // Required to avoid shadow mapping artifacts
            vkCmdSetDepthBias(
                    commandBuffers[i],
                    1.25f,
                    0.0f,
                    1.75f);

            for (auto& object : scene->getActiveDrawableObjects()) {
                if (object->shadowMaterial == nullptr) {
                    continue;
                }

                vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, object->shadowMaterial->graphicsPipeline);
                vkCmdBindDescriptorSets(commandBuffers[i],
                        VK_PIPELINE_BIND_POINT_GRAPHICS, object->shadowMaterial->pipelineLayout, 0, 1,
                                        &object->offscreenDescriptorSets[i], 0, nullptr);
                object->draw(commandBuffers[i], i);
            }

            vkCmdEndRenderPass(commandBuffers[i]);
        }

        {
            VkRenderPassBeginInfo renderPassInfo = {};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = renderPass;
            renderPassInfo.framebuffer = swapChainFramebuffers[i];
            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = swapchain.getExtents();

            clearValues[0].color = {0.0f, 0.7f, 1.0f, 1.0f};
            clearValues[1].depthStencil = {1.0f, 0};

            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            for (auto& object : scene->getActiveDrawableObjects()) {
                //TODO: Group objects by pipeline, bind pipeline and draw grouped objects
                vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
                                  object->material->graphicsPipeline);
                vkCmdBindDescriptorSets(commandBuffers[i],
                                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                                        object->material->pipelineLayout,
                                        0, 1, &object->descriptorSets[i],
                                        0, nullptr
                );
                object->draw(commandBuffers[i], i);
            }
       
            gui->drawFrame(commandBuffers[i]);
            vkCmdEndRenderPass(commandBuffers[i]);
        }

        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }
}

void Renderer::render() {
	vkWaitForFences(logicalDevice->getDevice(), 1, &inFlightFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());

	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(logicalDevice->getDevice(), swapchain.getSwapchain(), std::numeric_limits<uint64_t>::max(), imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

    gui->newFrame();
    gui->updateBuffers();

    if (recreateCommandBufferFlag) {
        recreateCommandBuffer();
    }

    auto camera = scene->getCamera();
    auto light = scene->getLight();
    auto& view = camera->getViewMatrix();
    auto& projection = camera->getProjectionMatrix();
    for (auto& object : scene->getActiveDrawableObjects()) {
        object->updateUniformBuffer(imageIndex, camera, light);
    }

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(logicalDevice->getDevice(), 1, &inFlightFences[currentFrame]);

    if (vkQueueSubmit(logicalDevice->getGraphicsQueue(), 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapchain.getSwapchain()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr; // Optional

    if (vkQueuePresentKHR(logicalDevice->getPresentQueue(), &presentInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Renderer::createSyncObjects() {
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(logicalDevice->getDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) !=
            VK_SUCCESS ||
            vkCreateSemaphore(logicalDevice->getDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) !=
            VK_SUCCESS ||
            vkCreateFence(logicalDevice->getDevice(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {

            throw std::runtime_error("failed to create synchronization game_objects for a frame!");
        }
    }
}

void Renderer::createGUI() {
    gui = new GUI(this, logicalDevice, window);
    gui->init(window->getExtents().width, window->getExtents().height);
    gui->initResources(renderPass, logicalDevice->getGraphicsQueue());
    gui->newFrame();
    gui->updateBuffers();
}

const VkDescriptorPool& Renderer::getDescriptorPool() const {
    return descriptorPool;
}

Swapchain* Renderer::getSwapchain() {
    return &swapchain;
}

const VkRenderPass& Renderer::getRenderPass() const {
    return renderPass;
}

const VkRenderPass& Renderer::getOffscreenRenderPass() const {
    return offscreenRenderPass;
}

void Renderer::setScene(Scene* newScene) {
    scene = newScene;
    createCommandbuffers();
    gui->setScene(newScene);
}

const VkExtent2D& Renderer::getExtents() const {
    return swapchain.getExtents();
}

GUI* Renderer::getGui() {
    return gui;
}
