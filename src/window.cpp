#include "window.h"
#include "game_objects/game_object.h"
#include "game_objects/model_object.h"
#include "game_objects/cube.h"
#include "materials/material.h"
#include "materials/basic_material.h"
#include "materials/basic_textured_material.h"
#include "materials/shadow_material.h"
#include "graphics/renderer.h"

Window::Window() {
    initWindow();
}

Window::~Window() {
    //cleanupSwapChain();

    glfwDestroyWindow(window);

    glfwTerminate();
}

void Window::initWindow() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

bool Window::shouldClose() {
	return glfwWindowShouldClose(window);
}

void Window::pollEvents() {
	glfwPollEvents();
}

void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	app->framebufferResized = true;
}

VkExtent2D Window::getExtents() const {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
    };

    return actualExtent;
}
