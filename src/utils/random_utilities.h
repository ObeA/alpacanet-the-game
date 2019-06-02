#pragma once

#include <random>

class RandomUtilities {
public:
    RandomUtilities(RandomUtilities const&) = delete;
    void operator=(RandomUtilities const&) = delete;

    static RandomUtilities& getInstance() {
        static RandomUtilities instance;

        return instance;
    }

    float getRandomBetween(float a, float b);
    int getRandomBetween(int a, int b);
private:
    RandomUtilities();

    std::random_device device;
    std::mt19937 engine;
};

