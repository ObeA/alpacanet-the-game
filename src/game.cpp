#include <thread>
#include "game.h"
#include "scenes/main_scene.h"
#include "managers/material_manager.h"
#include "graphics/logical_device.h"

Game::Game() : current_scene(nullptr) {
    graphics = new Graphics;
}

Game::~Game() {
    for (auto scene : scenes) {
        delete scene;
    }

    delete graphics;
}

void Game::run() {
    using namespace std::chrono_literals;
    auto& materialManager = MaterialManager::getInstance();

    materialManager.setup();
    setup();

    try {

        std::chrono::nanoseconds lag(0ns);
        auto oldTime = std::chrono::high_resolution_clock::now();

        while (!graphics->getWindow()->shouldClose()) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto delta = currentTime - oldTime;
            oldTime = currentTime;
            lag += std::chrono::duration_cast<std::chrono::nanoseconds>(delta);

            graphics->getWindow()->pollEvents();

            while (lag >= Timestep) {
                getCurrentScene()->update();
                lag -= Timestep;
            }

            graphics->getRenderer()->render();
        }

        vkDeviceWaitIdle(graphics->getLogicalDevice()->getDevice());
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    materialManager.cleanup();
}

void Game::setup() {
    auto mainScene = new MainScene(this);

    scenes.push_back(mainScene);
    current_scene = mainScene;
    current_scene->setup();

    graphics->getRenderer()->setScene(current_scene);
}

Scene* Game::getCurrentScene() {
    return current_scene;
}

Graphics* Game::getGraphics() {
    return graphics;
}
