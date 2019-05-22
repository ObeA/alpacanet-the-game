#include "camera.h"
#include <glm/gtx/norm.hpp>
#include "../game_objects/game_object.h"

Camera::Camera(Game* game, glm::vec3 position, float distance)
        : game(game), position(position), followDistance(distance), horizontalRotation(0) {
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

void Camera::update() {
    if (followedObject != nullptr) {
        targetPosition = followedObject->position;
    }

    position.x = std::cos(horizontalRotation) * followDistance - std::sin(horizontalRotation) * followDistance + targetPosition.x;
    position.y = std::sin(horizontalRotation) * followDistance + std::cos(horizontalRotation) * followDistance + targetPosition.y;
    position.z = targetPosition.z + followDistance;

    horizontalRotation += 0.01f;

    view = glm::lookAt(position, targetPosition, glm::vec3(0, 0, 1));
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

void Camera::onMouseMove(double x, double y) {
    currentMousePosition.x = x;
    currentMousePosition.y = y;
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

void Camera::lookAt(const GameObject* object) {
    followedObject = object;
}

void Camera::lookAt(const glm::vec3& newTargetPosition) {
    followedObject = nullptr;
    targetPosition = newTargetPosition;
}

void Camera::setHorizontalRotation(float radians) {
    horizontalRotation = radians;
}

void Camera::setFollowDistance(float newFollowDistance) {
    followDistance = newFollowDistance;
}
