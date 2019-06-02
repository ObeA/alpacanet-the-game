#include "main_scene.h"

#include <glm/gtx/intersect.hpp>

#include "../game_objects/model_object.h"
#include "../game_objects/particlesystem.h"
#include "../materials/basic_material.h"
#include "../materials/basic_textured_material.h"
#include "../materials/shadow_material.h"
#include "../materials/particle_material.h"
#include "scene_objects/alpaca.h"
#include "../managers/material_manager.h"
#include "../graphics/gui/gui.h"
#include "../game_objects/light.h"
#include "../utils/random_utilities.h"

void MainScene::setup() {
    auto& materialManager = MaterialManager::getInstance();
    auto graphics = game->getGraphics();

    materialManager.registerMaterial("basic-material", std::make_shared<BasicMaterial>(graphics, "basic"));
    materialManager.registerMaterial("shadow-material", std::make_shared<ShadowMaterial>(graphics));
    materialManager.registerMaterial("particle-material", std::make_shared<ParticleMaterial>(graphics, "particle"));
    materialManager.registerMaterial("skybox-material", std::make_shared<BasicTexturedMaterial>(graphics, (char*) "assets/textures/cube.png", "basictexturedambient"));

    auto world = new ModelObject(game, materialManager.getMaterial("basic-material").get(), materialManager.getMaterial("shadow-material").get(), (char*) "assets/models/world.obj");
    world->position = glm::vec3(0, 0, 0);
    objects.push_back(world);

    auto skybox = new ModelObject(game, materialManager.getMaterial("skybox-material").get(), materialManager.getMaterial("shadow-material").get(), (char*) "assets/models/cubemap.obj");
    skybox->scale = glm::vec3(50);
    objects.push_back(skybox);

    for (size_t i = 0; i < 5; i++) {
        auto alpaca = new Alpaca(game, materialManager.getMaterial("basic-material").get(), materialManager.getMaterial("shadow-material").get());
        alpaca->position = glm::vec3(getRandomPositionOnField(), 0.0);
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

    camera = new Camera(game, glm::vec3(5.0, 5.0, 5.0), 15.0f);
    lookAtWorld();

    light = new Light(game, glm::vec3(10, 10, 25), glm::vec3(0), 90.0f, 1.0f, 96.0f);
}

MainScene::~MainScene() {
    for (auto object : objects) {
        delete object;
    }
}

void MainScene::update() {
    Scene::update();

    auto it = std::begin(objects);

    while(it != std::end(objects)) {
        auto castedAlpaca = dynamic_cast<Alpaca*>(*it);
        if (castedAlpaca != nullptr) {
            if (castedAlpaca->hasReachedTargetPosition()) {
                if (castedAlpaca->nextMoveTick == -1) {
                    // Alpaca has reached target position. Let it idle there for a while.
                    castedAlpaca->nextMoveTick = game->currentTick() + RandomUtilities::getInstance().getRandomBetween(200, 1000);
                } else if (game->currentTick() > castedAlpaca->nextMoveTick) {
                    // Send alpaca to the next position
                    castedAlpaca->moveTo(getRandomPositionOnField());
                    castedAlpaca->nextMoveTick = -1;
                }
            }
        }

        auto castedParticleSystem = dynamic_cast<ParticleSystem*>(*it);
        if (castedParticleSystem != nullptr && castedParticleSystem->destroyFlag) {
            auto prevIt = *it;
            it = objects.erase(it);
            delete prevIt;
        }
        else {
            ++it;
        }
    }
}

void MainScene::onMouseButton(int button, int action, int mods) {
    if (button != GLFW_MOUSE_BUTTON_LEFT && action != GLFW_PRESS) {
        return;
    }

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
            lookAtWorld();
            break;
        case GLFW_KEY_Z:
            loopAlpacas(false);
            break;
        case GLFW_KEY_X:
            shearSelectedAlpaca();
            break;
        case GLFW_KEY_C:
            loopAlpacas(true);
            break;
    }
}

void MainScene::loopAlpacas(bool nextOrPrevious) {
    auto alpacas = getAlpacas();
    if (selectedAlpaca) {
        auto it = std::begin(alpacas);

        while (it != std::end(alpacas)) {
            if (*it == selectedAlpaca) {
                if (it == std::begin(alpacas) && !nextOrPrevious)
                    it = std::prev(std::end(alpacas));
                else if (std::next(it) == std::end(alpacas) && nextOrPrevious)
                    it = (std::begin(alpacas));
                else
                    it = nextOrPrevious ? ++it : --it;
                selectedAlpaca = dynamic_cast<Alpaca*>(*it);
                break;
            }
            ++it;
        }
    } else {
        selectedAlpaca = alpacas[0];
    }
    camera->lookAt(selectedAlpaca);
    camera->setFollowDistance(2.0f);
}

void MainScene::drawUI() {
    ImGui::SetNextWindowPos(ImVec2(525, 425), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(250, 150), ImGuiCond_Always);
    ImGui::Begin("Huidige alpaca", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    if (selectedAlpaca != nullptr) {
        ImGui::TextUnformatted(("Wol: " + std::to_string(selectedAlpaca->getWooliness()) + "\t Blij: " + std::to_string(selectedAlpaca->getHappiness())).c_str());
    }
    else {
        ImGui::TextUnformatted("Geen alpaca geselecteerd");
    }

    ImGui::TextUnformatted("Wool-O-Meter");
    ImGui::ProgressBar(score / (float)1000, ImVec2(-1, 0), (std::to_string(score) + "/1000").c_str());

    ImGui::End();
}

void MainScene::lookAtWorld() {
    camera->lookAt(glm::vec3(0));
    camera->setFollowDistance(10.0f);
}

void MainScene::shearSelectedAlpaca() {
    if (selectedAlpaca == nullptr) {
        return;
    }

    auto wool = selectedAlpaca->shear();
    score += wool;
    if (wool > 0) {
        auto& materialManager = MaterialManager::getInstance();
        auto particles = new ParticleSystem(game, materialManager.getMaterial("particle-material").get(), nullptr);
        particles->amount = wool;
        auto center = selectedAlpaca->getBounds().getCenter();
        particles->position = selectedAlpaca->position + glm::vec3(center.x, center.y, center.z * .75);
        particles->scale = glm::vec3(1);
        objects.push_back(particles);
        particles->start();
        game->getGraphics()->getRenderer()->recreateCommandBufferFlag = true;
    }
}

glm::vec2 MainScene::getRandomPositionOnField() {
    auto& random = RandomUtilities::getInstance();
    float x = random.getRandomBetween(-8.5f, 8.5f);
    float y = random.getRandomBetween(-8.5f, 8.5f);

    return glm::vec2(x, y);
}