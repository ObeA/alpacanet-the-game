#include "camera.h"
#include <glm/gtx/norm.hpp>
#include "../game_objects/game_object.h"

Camera::Camera(Game* game, glm::vec3 position, float yaw, float pitch)
        : game(game), position(position), yaw(yaw), pitch(pitch) {
    update();

    auto extents = game->getGraphics()->getRenderer()->getExtents();
    projection = glm::perspective(glm::radians(90.0f), (float)extents.width / extents.height, 0.1f, 20.0f);

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

const glm::mat4& Camera::getProjectionMatrix() const {
    return projection;
}

const glm::vec3& Camera::getPosition() const {
    return position;
}

void Camera::lookAt(GameObject* object) {
    followedObject = object;
}

void Camera::update() {
    if (followedObject != nullptr) {
        position = followedObject->position;
        position += glm::vec3(3.0f);
        view = glm::lookAt(position, followedObject->position, glm::vec3(0, 0, 1));
    } else {
        if (glm::length2(currentMousePosition - previousMousePosition) > 0) {
            if (currentMousePosition.x == 0 && currentMousePosition.y == 0) {
                previousMousePosition = currentMousePosition;
            } else {
                auto delta = currentMousePosition - previousMousePosition;
                previousMousePosition = currentMousePosition;
                float sensitivity = 0.5;
                yaw -= (delta.x * sensitivity);
                pitch = std::min(89.0f, std::max(-89.0f, pitch + (delta.y * sensitivity)));
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
}

glm::vec3 Camera::getRay() {
    float mouseX = currentMousePosition.x / (game->getGraphics()->getWindow()->getExtents().width * 0.5f) - 1.0f;
    float mouseY = currentMousePosition.y / (game->getGraphics()->getWindow()->getExtents().height * 0.5f) - 1.0f;

    glm::mat4 invVP = glm::inverse(projection * view);
    glm::vec4 screenPos = glm::vec4(mouseX, -mouseY, 1.0f, 1.0f);
    glm::vec4 worldPos = invVP * screenPos;

    glm::vec3 dir = glm::normalize(glm::vec3(worldPos));

    return dir;
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
