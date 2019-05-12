#include <iostream>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "Game.h"


int main() {
    Game game = Game();

    game.run();
    return 0;
}