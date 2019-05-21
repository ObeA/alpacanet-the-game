#pragma once

#include <glm/glm.hpp>
#include "../game.h"

constexpr glm::vec3 WORLD_UP{0.0, 0.0, 1.0};

class Game;
class GameObject;

class Camera {
public:
    Camera(Game* game, glm::vec3 position, float yaw, float pitch);

    void setPosition(glm::vec3 position);
    void lookAt(GameObject* object);
    void setRotation(float yaw, float pitch);

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
    glm::vec3 up{};
    glm::vec3 forward{};
    glm::vec3 right{};

    float yaw;
    float pitch;

    glm::vec2 previousMousePosition{};
    glm::vec2 currentMousePosition{};
    glm::vec3 moveDirection{};

    GameObject* followedObject = nullptr;

    void onMouseMove(double x, double y);

    void onKeyDown(int key, int scancode, int mods);

    void onKeyUp(int key, int scancode, int mods);
};

