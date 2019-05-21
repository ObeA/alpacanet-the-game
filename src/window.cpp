#include "window.h"
#include "game_objects/game_object.h"
#include "game_objects/model_object.h"
#include "game_objects/cube.h"
#include "materials/material.h"
#include "materials/basic_material.h"
#include "materials/basic_textured_material.h"
#include "materials/shadow_material.h"
#include "graphics/renderer.h"

Window::Window() : onCursorMoveCallbacks{} {
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

	window = glfwCreateWindow(WIDTH, HEIGHT, "AlpacaWeb: The Game", nullptr, nullptr);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

	glfwSetCursorPosCallback(window, cursorPositionCallback);
	glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

bool Window::shouldClose() {
	return glfwWindowShouldClose(window);
}

void Window::pollEvents() {
	glfwPollEvents();
}

void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	auto self = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	self->framebufferResized = true;
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

void Window::registerOnCursorMoveCallback(std::function<void(double, double)> callback) {
    onCursorMoveCallbacks.emplace_back(callback);
}

void Window::registerOnKeyDownCallback(std::function<void(int, int, int)> callback) {
    onKeyDownCallbacks.emplace_back(callback);
}

void Window::registerOnKeyUpCallback(std::function<void(int, int, int)> callback) {
    onKeyUpCallbacks.emplace_back(callback);
}

void Window::registerOnMouseButtonCallback(std::function<void(int, int, int)> callback) {
    onMouseButtonCallbacks.emplace_back(callback);
}

void Window::cursorPositionCallback(GLFWwindow* window, double xoffset, double yoffset) {
    auto self = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

    for (const auto& callback : self->onCursorMoveCallbacks) {
        callback(xoffset, yoffset);
    }
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        auto self = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

        for (const auto& callback : self->onMouseButtonCallbacks) {
            callback(button, action, mods);
        }
    }
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto self = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

    std::vector<std::function<void(int, int, int)>> callbacks;
    if (action == GLFW_PRESS) {
        callbacks = self->onKeyDownCallbacks;
    } else if (action == GLFW_RELEASE) {
        callbacks = self->onKeyUpCallbacks;
    }

    for (const auto& callback : callbacks) {
        callback(key, scancode, mods);
    }
}