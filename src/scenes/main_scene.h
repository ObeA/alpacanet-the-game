#pragma once

#include "scene.h"
#include "scene_objects/alpaca.h"
#include "../game_objects/particlesystem.h"

class MainScene : public Scene {
public:
    explicit MainScene(Game* game) : Scene(game) { }
    ~MainScene() override;

    void setup() override;
    void update() override;
    void onMouseButton(int button, int action, int mods);
    void onKeyDown(int key, int scancode, int mods);
    void drawUI() override;
    std::vector<Alpaca*> getAlpacas();
private:
    inline static Bounds FIELD_BOUNDS = Bounds(glm::vec3(-8.5, -8.5, 0.0f), glm::vec3(8.5, 8.5, 0.0f));

    Alpaca* selectedAlpaca = nullptr;
    int score = 0;
    int goal = 500;
    int previousGoal = 0;
    int goalsReached = 0;
    void loopAlpacas(bool nextOrPrevious);

    void shearSelectedAlpaca();

    void lookAtWorld();

    glm::vec2 getRandomPositionWithIn(const Bounds& bounds) const;

    ParticleSystem* SpawnParticleSystem(const glm::vec3 &position, int count);

    Alpaca* SpawnAlpaca(const Bounds &bounds = FIELD_BOUNDS);
};