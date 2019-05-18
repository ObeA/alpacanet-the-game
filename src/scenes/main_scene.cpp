#include "main_scene.h"
#include "../game_objects/model_object.h"
#include "../materials/basic_material.h"
#include "../materials/basic_textured_material.h"
#include "../materials/shadow_material.h"

void MainScene::setup() {
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

	for (auto object : objects) {
	    object->start();
	}
}

MainScene::~MainScene() {
    for (auto material : materials) {
        delete material;
    }

    for (auto object : objects) {
        delete object;
    }
}

void MainScene::update() {

}
