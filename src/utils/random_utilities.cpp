#include "random_utilities.h"

RandomUtilities::RandomUtilities() : device(), engine(device()) {

}

float RandomUtilities::getRandomBetween(float a, float b) {
    return std::uniform_real_distribution<float>(a, b)(engine);
}

int RandomUtilities::getRandomBetween(int a, int b) {
    return std::uniform_int_distribution<int>(a, b)(engine);
}
