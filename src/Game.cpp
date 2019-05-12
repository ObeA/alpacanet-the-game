#include "Game.h"

void Game::run() {
    try {
        window.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}