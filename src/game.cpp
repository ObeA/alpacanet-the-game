#include "game.h"
#include "scenes/main_scene.h"
#include "managers/material_manager.h"
#include "graphics/instance.h"
#include "graphics/surface.h"
#include "graphics/physical_device.h"
#include "graphics/logical_device.h"

Game::Game() : graphics(new Graphics()), current_scene(nullptr) {
}

Game::~Game() {
    for (auto scene : scenes) {
        delete scene;
    }

    delete graphics;
}

void Game::run() {
    auto& materialManager = MaterialManager::getInstance();

    materialManager.setup();
    setup();

    try {
        graphics->getWindow()->initWindow();

		while (!graphics->getWindow()->shouldClose()) {
			graphics->getWindow()->pollEvents();
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
