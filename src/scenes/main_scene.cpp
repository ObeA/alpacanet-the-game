#include "main_scene.h"
#include "../game_objects/model_object.h"
#include "../materials/basic_material.h"
#include "../materials/basic_textured_material.h"
#include "../materials/shadow_material.h"
#include "scene_objects/alpaca.h"
#include <glm/gtx/intersect.hpp>

void MainScene::setup() {
    camera = new Camera(game, glm::vec3(2.0, 2.0, 2.0), 0, 225);

    auto graphics = game->getGraphics();
    materials.push_back(new BasicMaterial(graphics));
	materials.push_back(new BasicTexturedMaterial(graphics, (char*)"assets/textures/texture.jpg"));
	materials.push_back(new BasicTexturedMaterial(graphics, (char*)"assets/textures/banana.jpg"));
    materials.push_back(new BasicTexturedMaterial(graphics, (char*)"assets/textures/chalet.jpg"));
	materials.push_back(new ShadowMaterial(graphics));
	for (auto& material : materials) {
		material->initialize();
	}

	auto model = new ModelObject(game, materials[2], materials[4], (char*)"assets/models/cube.obj");
    model->scale = glm::vec3(.5);
	model->position = glm::vec3(0, -2, 1);

	auto model2 = new ModelObject(game, materials[1], materials[4], (char*)"assets/models/cube.obj");
    model2->scale = glm::vec3(.5);
    model2->position = glm::vec3(0, 0, 2);
    objects.push_back(model);
	objects.push_back(model2);


    for (size_t i = 0; i < 5; i++)
    {
        auto alpaca = new Alpaca(game, materials[1], materials[4]);
        alpaca->position = glm::vec3(0);
        alpaca->scale = glm::vec3(1, 0.5, 0.5);
        objects.push_back(alpaca);
    }

	for (auto object : objects) {
	    object->start();
	}

    auto window = game->getGraphics()->getWindow();
    auto onMouseButtonCallback = std::bind(&MainScene::onMouseButton, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    window->registerOnMouseButtonCallback(onMouseButtonCallback);
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
    camera->update();

    auto currentTime = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()
        );

    for (auto object : objects) {
        auto casted = dynamic_cast<Alpaca*>(object);
        if (casted != nullptr) {
            casted->update();
            if (currentTime > casted->nextMoveTime) {
                casted->nextMoveTime = currentTime + std::chrono::seconds(5 + std::rand() % 10);
                auto newPosition = glm::vec3(glm::vec2((std::rand() % 10) - 5, (std::rand() % 10) - 5), 1.0);
                casted->moveTo(newPosition);
            };
        }
    }
}

void MainScene::onMouseButton(int button, int action, int mods) {
    auto ray = camera->getRay();

    for (auto object : objects) {
        float distance;
        auto intersected = glm::intersectRaySphere(camera->getPosition(), ray, object->position, 1, distance);
        if (intersected) 
        {
            object->scale = glm::vec3(.1);
        }
    }
}
