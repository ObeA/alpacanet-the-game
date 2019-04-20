//
// Created by Leon on 4/20/2019.
//

#include "Game.h"

void Game::run() {
    mainLoop();
}

void Game::mainLoop() {
    while (!glfwWindowShouldClose(window.GLFWWindow)) {
        glfwPollEvents();
    }
}
