#include <iostream>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "src/Game.h"


int main() {
    glm::vec3 alpaca;
    glm::mat4 henk = glm::translate(glm::mat4(1.0f), alpaca);

    std::cout << "hallo";

    Game game = Game();

    game.run();
    return 0;
}