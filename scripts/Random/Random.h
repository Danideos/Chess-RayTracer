#ifndef MAIN_CPP_RANDOM_H
#define MAIN_CPP_RANDOM_H

#include "../Utilities/Utils.h"

class Rand {
public:
    static void Initialize();
    static double RandomDouble();
    static Vec3D RandomUnitVector();

private:
    static thread_local std::mt19937 randomGenerator_;
    static std::uniform_int_distribution<std::mt19937::result_type> distribution_;
    static std::uniform_int_distribution<std::mt19937::result_type> cachedDistribution_;
};

#endif
