#include "../Utilities/Utils.h"
#include "Random.h"

thread_local std::mt19937 Rand::randomGenerator_{std::random_device{}()};
std::uniform_int_distribution<std::mt19937::result_type> Rand::distribution_{std::numeric_limits<std::mt19937::result_type>::min(), std::numeric_limits<std::mt19937::result_type>::max()};
std::uniform_int_distribution<std::mt19937::result_type> Rand::cachedDistribution_{0, 10000 - 1};

double Rand::RandomDouble() {
    return (double)distribution_(randomGenerator_) / (double)std::numeric_limits<std::mt19937::result_type>::max();
}

Vec3D Rand::RandomUnitVector() {
    Vec3D randomVector{RandomDouble() - 0.5, RandomDouble() - 0.5, RandomDouble() - 0.5};
    randomVector.normalize();
    return randomVector;
}

void Rand::Initialize() {}