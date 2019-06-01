#include "light.h"

Light::Light(Game* game, glm::vec3 position, glm::vec3 target, float fov, float near, float far)
    : GameObject(game, position), target(target), fov(fov), near(near), far(far) {

}

const glm::mat4 Light::getProjectionMatrix() const {
    return glm::perspective(glm::radians(fov), 1.0f, near, far);
}

const glm::mat4 Light::getViewMatrix(const glm::vec3& up) const {
    return glm::lookAt(position, target, up);
}
