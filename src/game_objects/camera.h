#pragma once

#include <glm/glm.hpp>
#include "../game.h"

class Game;
class GameObject;

class Camera {
public:
    const glm::vec3 up = glm::vec3(0, 0, 1);

    Camera(Game* game, glm::vec3 position, float followDistance);

    void lookAt(const GameObject* object);
    void lookAt(const glm::vec3& position);
    void setHorizontalRotation(float radians);
    void setFollowDistance(float newFollowDistance);

    void update();

    glm::vec3 getRay();

    const glm::mat4& getViewMatrix() const;
    const glm::mat4& getProjectionMatrix() const;
    const glm::vec3& getPosition() const;

private:
    Game* game;

    glm::mat4 view{};
    glm::mat4 projection{};

    glm::vec3 position;

    float followDistance;
    float horizontalRotation;

    bool isDragging = false;
    glm::vec2 dragPosition;
    glm::vec2 currentMousePosition;

    const GameObject* followedObject = nullptr;
    glm::vec3 offset{};
    glm::vec3 targetPosition;

    void onMouseMove(double x, double y);

    void onKeyDown(int key, int scancode, int mods);

    void onKeyUp(int key, int scancode, int mods);

    void onMouseButton(int button, int action, int mods);
};

