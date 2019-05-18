#include "camera.h"
#include <glm/gtx/norm.hpp>

Camera::Camera(Game* game, glm::vec3 position, float yaw, float pitch)
        : game(game), position(position), yaw(yaw), pitch(pitch) {
    update();

    auto window = game->getGraphics()->getWindow();
    auto onMoveCallback = std::bind(&Camera::onMouseMove, this, std::placeholders::_1, std::placeholders::_2);
    window->registerOnCursorMoveCallback(onMoveCallback);

    auto onKeyDownCallback = std::bind(&Camera::onKeyDown, this, std::placeholders::_1, std::placeholders::_2,
                                       std::placeholders::_3);
    window->registerOnKeyDownCallback(onKeyDownCallback);

    auto onKeyUpCallback = std::bind(&Camera::onKeyUp, this, std::placeholders::_1, std::placeholders::_2,
                                     std::placeholders::_3);
    window->registerOnKeyUpCallback(onKeyUpCallback);
}

const glm::mat4& Camera::getViewMatrix() const {
    return view;
}

void Camera::update() {
    if (glm::length2(currentMousePosition - previousMousePosition) > 0) {
        if (currentMousePosition.x == 0 && currentMousePosition.y == 0) {
            previousMousePosition = currentMousePosition;
        } else {
            auto delta = currentMousePosition - previousMousePosition;
            std::cout << delta.x << " " << delta.y << " from " << "(" << previousMousePosition.x << "," << previousMousePosition.y << ")" << " to " << "(" << currentMousePosition.x << "," << currentMousePosition.y << ")" << std::endl;
            previousMousePosition = currentMousePosition;
            yaw += delta.x;
            pitch += std::min(89.0f, std::max(-89.0f, pitch + delta.y));
        }
    }

    auto velocity = 0.005f * (Game::Timestep.count() / 1000000.0f);
    position += forward * moveDirection.y * velocity;
    position += right * moveDirection.x * velocity;

    forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward.z = sin(glm::radians(pitch));
    forward.y = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward = glm::normalize(forward);

    right = glm::normalize(glm::cross(forward, WORLD_UP));
    up = glm::normalize(glm::cross(right, forward));

    view = glm::lookAt(position, position + forward, up);
}

void Camera::setPosition(glm::vec3 newPosition) {
    position = newPosition;
    update();
}

void Camera::setRotation(float newYaw, float newPitch) {
    yaw = newYaw;
    pitch = newPitch;
    update();
}

void Camera::onMouseMove(double x, double y) {
    currentMousePosition.x = x;
    currentMousePosition.y = y;
    update();
}

void Camera::onKeyDown(int key, int scancode, int mods) {
    switch (key) {
        case GLFW_KEY_W:
            moveDirection.y = 1;
            break;
        case GLFW_KEY_S:
            moveDirection.y = -1;
            break;
        case GLFW_KEY_A:
            moveDirection.x = -1;
            break;
        case GLFW_KEY_D:
            moveDirection.x = 1;
            break;
    }
}

void Camera::onKeyUp(int key, int scancode, int mods) {
    switch (key) {
        case GLFW_KEY_W:
        case GLFW_KEY_S:
            moveDirection.y = 0;
            break;
        case GLFW_KEY_A:
        case GLFW_KEY_D:
            moveDirection.x = 0;
            break;
    }
}
