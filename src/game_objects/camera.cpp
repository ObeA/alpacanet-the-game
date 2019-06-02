#include "camera.h"
#include <glm/gtx/norm.hpp>
#include "../game_objects/game_object.h"
#include "../game_objects/drawable_object.h"

Camera::Camera(Game* game, glm::vec3 position, float distance)
        : game(game), position(position), followDistance(distance), horizontalRotation(0) {
    auto extents = game->getGraphics()->getRenderer()->getExtents();
    projection = glm::perspective(glm::radians(90.0f), (float)extents.width / extents.height, 0.1f, 100.0f);

    auto window = game->getGraphics()->getWindow();
    auto onMoveCallback = std::bind(&Camera::onMouseMove, this, std::placeholders::_1, std::placeholders::_2);
    window->registerOnCursorMoveCallback(onMoveCallback);

    auto onKeyDownCallback = std::bind(&Camera::onKeyDown, this, std::placeholders::_1, std::placeholders::_2,
                                       std::placeholders::_3);
    window->registerOnKeyDownCallback(onKeyDownCallback);

    auto onKeyUpCallback = std::bind(&Camera::onKeyUp, this, std::placeholders::_1, std::placeholders::_2,
                                     std::placeholders::_3);
    window->registerOnKeyUpCallback(onKeyUpCallback);

    auto onMouseButtonCallback = std::bind(&Camera::onMouseButton, this, std::placeholders::_1,
                                           std::placeholders::_2, std::placeholders::_3);
    window->registerOnMouseButtonCallback(onMouseButtonCallback);
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
        targetPosition = followedObject->position + offset;
    }

    position.x = std::cos(horizontalRotation) * followDistance - std::sin(horizontalRotation) * followDistance + targetPosition.x;
    position.y = std::sin(horizontalRotation) * followDistance + std::cos(horizontalRotation) * followDistance + targetPosition.y;
    position.z = targetPosition.z + followDistance;

    if (isDragging) {
        auto delta = currentMousePosition - dragPosition;

        horizontalRotation += delta.x * 0.01f;
        dragPosition = currentMousePosition;
    }

    view = glm::lookAt(position, targetPosition, up);
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

void Camera::onMouseButton(int button, int action, int mods) {
    if (button != GLFW_MOUSE_BUTTON_RIGHT) {
        return;
    }

    isDragging = action == GLFW_PRESS;
    if (isDragging) {
        dragPosition = currentMousePosition;
    }
}

void Camera::onKeyDown(int key, int scancode, int mods) {

}

void Camera::onKeyUp(int key, int scancode, int mods) {

}

void Camera::lookAt(const GameObject* object) {
    followedObject = object;
    offset = glm::vec3(0);

    /*auto drawable = dynamic_cast<const DrawableObject*>(object);
    if (drawable != nullptr) {
        offset = drawable->getBounds().getCenter();
    }*/
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
