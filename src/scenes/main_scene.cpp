#include "main_scene.h"
#include "../game_objects/model_object.h"
#include "../materials/basic_material.h"
#include "../materials/basic_textured_material.h"
#include "../materials/shadow_material.h"
#include "scene_objects/alpaca.h"
#include "../graphics/gui/gui.h"
#include <glm/gtx/intersect.hpp>

void MainScene::setup() {
    auto graphics = game->getGraphics();
    materials.push_back(new BasicMaterial(graphics));
    materials.push_back(new BasicTexturedMaterial(graphics, (char*) "assets/textures/texture.jpg"));
    materials.push_back(new BasicTexturedMaterial(graphics, (char*) "assets/textures/banana.jpg"));
    materials.push_back(new BasicTexturedMaterial(graphics, (char*) "assets/textures/chalet.jpg"));
    materials.push_back(new ShadowMaterial(graphics));
    for (auto& material : materials) {
        material->initialize();
    }

    auto model = new ModelObject(game, materials[1], materials[4], (char*) "assets/models/cube.obj");
    model->scale = glm::vec3(.5);
    model->position = glm::vec3(0, -2, 1);

    auto model2 = new ModelObject(game, materials[2], materials[4], (char*) "assets/models/cube.obj");
    model2->scale = glm::vec3(20, 20, .2);
    model2->position = glm::vec3(0, 0, -.5);
    objects.push_back(model);
    objects.push_back(model2);


    auto test = new Alpaca(game, materials[1], materials[4]);
    test->position = glm::vec3(0);
    test->scale = glm::vec3(1, 0.5, 0.5);
    test->start();
    for (size_t i = 0; i < 5; i++) {
        auto alpaca = new Alpaca(game, materials[1], materials[4]);
        alpaca->position = glm::vec3(0);
        alpaca->scale = glm::vec3(1, 0.5, 0.5);
        objects.push_back(alpaca);
    }

    for (auto object : objects) {
        object->start();
    }

    auto window = game->getGraphics()->getWindow();
    auto onMouseButtonCallback = std::bind(&MainScene::onMouseButton, this, std::placeholders::_1,
                                           std::placeholders::_2, std::placeholders::_3);
    window->registerOnMouseButtonCallback(onMouseButtonCallback);

    auto onKeyDownCallback = std::bind(&MainScene::onKeyDown, this, std::placeholders::_1, std::placeholders::_2,
                                       std::placeholders::_3);
    window->registerOnKeyDownCallback(onKeyDownCallback);

    camera = new Camera(game, glm::vec3(0.0, 0.0, 0.0), 3.0f);
    camera->lookAt(test);
}

MainScene::~MainScene() {
    for (auto material : materials) {
        material->cleanup();
    }

    for (auto object : objects) {
        delete object;
    }
}

void MainScene::update() {
    auto currentTime = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );

    for (auto object : objects) {
        auto casted = dynamic_cast<Alpaca*>(object);
        if (casted != nullptr) {
            casted->update();
            if (currentTime > casted->nextMoveTime) {
                casted->nextMoveTime = currentTime + std::chrono::seconds(5 + std::rand() % 10);
                auto newPosition = glm::vec3(glm::vec2((std::rand() % 20) - 10, (std::rand() % 20) - 10), 1.0);
                casted->moveTo(newPosition);
            }
        }
    }

    camera->update();
}

void MainScene::onMouseButton(int button, int action, int mods) {
    auto ray = camera->getRay();

    for (auto object : objects) {
        float distance;
        auto intersected = glm::intersectRaySphere(camera->getPosition(), ray, object->position, 2, distance);
        if (intersected) {
            auto casted = dynamic_cast<Alpaca*>(object);
            if (casted != nullptr) {
                selectedAlpaca = casted;
                camera->lookAt(casted);
                camera->setFollowDistance(2.0f);
                game->getGraphics()->getRenderer()->getGui()->setAlpaca(casted);
            }
        }
    }
}

std::vector<Alpaca*> MainScene::getAlpacas() {
    auto alpacas = std::vector<Alpaca*>();
    for (auto object : objects) {
        auto casted = dynamic_cast<Alpaca*>(object);
        if (casted != nullptr) {
            alpacas.push_back(casted);
        }
    }
    return alpacas;
}

void MainScene::onKeyDown(int key, int scancode, int mods) {
    switch (key) {
        case GLFW_KEY_ESCAPE:
            camera->lookAt(glm::vec3(0));
            camera->setFollowDistance(4.0f);
            break;
        case GLFW_KEY_Z:
            loopAlpacas(false);
            break;
        case GLFW_KEY_X:
            if (selectedAlpaca) {
                auto wool = selectedAlpaca->shear();
                score += wool;
                std::cout << "sheared" << wool << "total is" << score << std::endl;
            }
            break;
        case GLFW_KEY_C:
            loopAlpacas(true);
            break;
    }
}

void MainScene::loopAlpacas(bool nextOrPrevious) {
    auto alpacas = getAlpacas();
    if (selectedAlpaca) {
        for (size_t i = 0; i < alpacas.size(); i++) {
            if (alpacas[i] == selectedAlpaca) {
                if (i == 0 && !nextOrPrevious)
                    selectedAlpaca = alpacas[alpacas.size() - 1];
                else if (i == alpacas.size() - 1 && nextOrPrevious)
                    selectedAlpaca = alpacas[0];
                else {
                    selectedAlpaca = nextOrPrevious ? alpacas[i + 1] : alpacas[i - 1];
                }
            }
        }
    } else {
        selectedAlpaca = alpacas[0];
    }
    camera->lookAt(selectedAlpaca);
    game->getGraphics()->getRenderer()->getGui()->setAlpaca(selectedAlpaca);
}
