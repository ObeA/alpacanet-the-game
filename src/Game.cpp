//
// Created by Leon on 4/20/2019.
//

#include "Game.h"

void Game::run() {
    try {
        window.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::cin.get();
    }
    std::cin.get();
}