#ifndef MAIN_CPP_UTILS_H
#define MAIN_CPP_UTILS_H

#include "../LinearAlgebra/Vector.h"

namespace Utils{
    constexpr const double RGB_MAX = 255.;

    // RayTrace params
    constexpr const int BOUNCES = 2; // not used currently

    // Ray params
    const Vec3D EMPTY_COLOR{0., 0., 0.}; // Ray doesn't intersect any object color
    constexpr const double PARALLEL_PRECISION = 1e-3;  // Ray parallel with plane precision

    // Light source params
    const Vec3D LIGHT_COLOR = Vec3D{1., 1., 1.};
    const Vec3D LIGHT_DIR = Vec3D{-1., -1., -1.};
    constexpr const double LIGHT_INTENSITY = 15;

    // Camera params
    const Vec3D DEFAULT_CAMERA_POS{4., 4., 16.};
    const Vec3D DEFAULT_CAMERA_LOOKAT{4., 2., 4.}; // Center of camera look at position
    const Vec3D DEFAULT_CAMERA_UP{0., 1., 0.}; // Fixates the up vector of screen

    constexpr const double DEFAULT_CAMERA_LENGTH = 1.; // Distance of screen from camera position
    constexpr const double DEFAULT_CAMERA_ASPECT = 16. / 9. ; // Aspect ratio of screen width to height
    constexpr const double DEFAULT_CAMERA_HORSCALE = 1. ; // Horizontal scale of screen width

    // Object params
    const Vec3D BASE_COLOR = Vec3D{1., 1., 1.};
    constexpr const double BASE_ALBEDO = 0.18; // 0.18 is the average (~) of real world objects

    // Multithreading
    #define __MT__
}

#endif