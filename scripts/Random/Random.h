#ifndef MAIN_CPP_RANDOM_H
#define MAIN_CPP_RANDOM_H

#include "../Utilities/Utils.h"

class Rand {
public:
    static void Initialize();
    /// \brief Returns a random double in the interval [0,1]
    static double RandomDouble();
    /// \brief Returns a random normalized vector
    static Vec3D RandomUnitVector();

private:
    static thread_local std::mt19937 randomGenerator_;
    static std::uniform_int_distribution<std::mt19937::result_type> distribution_;
    static std::uniform_int_distribution<std::mt19937::result_type> cachedDistribution_;
};

#endif
