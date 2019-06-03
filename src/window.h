#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <map>
#include <optional>
#include <functional>
#include <cstdlib>
#include <set>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <array>
#include <cstring>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "models/vertex.h"
#include "models/particle.h"
#include "utils/file_utilities.h"

const int WIDTH = 1366;
const int HEIGHT = 768;

const int MAX_FRAMES_IN_FLIGHT = 2;

struct UniformBufferObjectOffscreen {
    glm::mat4 depthMVP;
};

class Window {
public:
    Window();
    ~Window();

    GLFWwindow* window;

	VkExtent2D getExtents() const;

	bool shouldClose();

	void pollEvents();

	void registerOnCursorMoveCallback(std::function<void(double, double)> callback);
    void registerOnKeyDownCallback(std::function<void(int, int, int)> callback);
    void registerOnKeyUpCallback(std::function<void(int, int, int)> callback);
    void registerOnMouseButtonCallback(std::function<void(int, int, int)> callback);

private:
    void initWindow();

    std::vector<std::function<void(double, double)>> onCursorMoveCallbacks;
    std::vector<std::function<void(int, int, int)>> onKeyDownCallbacks;
    std::vector<std::function<void(int, int, int)>> onKeyUpCallbacks;
    std::vector<std::function<void(int, int, int)>> onMouseButtonCallbacks;

	static void cursorPositionCallback(GLFWwindow* window, double x, double y);
    static void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};