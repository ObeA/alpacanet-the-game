#include "game.h"
#include "scenes/main_scene.h"
#include "managers/material_manager.h"
#include "graphics/instance.h"
#include "graphics/surface.h"
#include "graphics/physical_device.h"
#include "graphics/logical_device.h"

Game::Game() : graphics(), current_scene(nullptr) {
}

Game::~Game() {
    for (auto scene : scenes) {
        delete scene;
    }
}

void Game::run() {
    auto& materialManager = MaterialManager::getInstance();

    materialManager.setup();
    setup();

    try {
        graphics.getWindow()->run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    materialManager.cleanup();
}

void Game::setup() {
    auto mainScene = new MainScene();

    scenes.push_back(mainScene);
    current_scene = mainScene;

    current_scene->setup();
}

Scene* Game::getCurrentScene() {
    return current_scene;
}

Graphics* Game::getGraphics() {
    return &graphics;
}
