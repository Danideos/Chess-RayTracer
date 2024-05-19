#ifndef MAIN_CPP_UTILS_H
#define MAIN_CPP_UTILS_H

#include "../LinearAlgebra/Vector.h"

namespace Utils{
    // RayTrace params
    constexpr const int BOUNCES = 10;
    constexpr const int SQRT_SAMPLES = 1;

    // Ray params
    constexpr const double PARALLEL_PRECISION = 1e-3;  // Ray parallel with plane detection precision

    // Light source params
    const Vec3D LIGHT_COLOR = Vec3D{1., 1., 1.};
    const Vec3D LIGHT_DIR = Vec3D{-1, -1., -1.};
    constexpr const double LIGHT_INTENSITY = 15;

    // Camera params
    const Vec3D DEFAULT_CAMERA_POS{6., 6., 16.};
    const Vec3D DEFAULT_CAMERA_LOOKAT{4., 0., 4.}; // Center of camera look at position
    const Vec3D DEFAULT_CAMERA_UP{0., 1., 0.}; // Fixates the up vector of screen

    constexpr const double DEFAULT_CAMERA_LENGTH = 1.; // Distance of screen from camera position
    constexpr const double DEFAULT_CAMERA_ASPECT = 16. / 9. ; // Aspect ratio of screen width to height
    constexpr const double DEFAULT_CAMERA_HORSCALE = 1. ; // Horizontal scale of screen width

    // Object params
    #define __SMOOTHING__ // Smooth triangle meshes(uses barycentric coordinates)
    const Vec3D BASE_ALBEDO = Vec3D{0.7, 0.7, 0.7}; // Unassigned color base
    constexpr const double BASE_REFRACTION_INDEX = 1.5;
    const Vec3D BASE_TRANSPARENCY = Vec3D{1., 1., 1.};
    constexpr const double BASE_SMOOTHNESS = 1.; // Unassigned smoothing factor from [0,1],
    constexpr const double BASE_FUZZ = 0.;

    // Color params
    constexpr const double RGB_MAX = 255.; // Don't change this one, just constant

    const Vec3D WHITE_BOARD_COLOR{1., 0.8 , .65};
    const Vec3D BLACK_BOARD_COLOR{0.5, .2 , 0.2};
    const Vec3D WHITE_PIECE_COLOR{1., 0.8 , 0.65};
    const Vec3D BLACK_PIECE_COLOR{0.5, .2 , 0.2};

//    const Vec3D BACKGROUND_COLOR{0.6, 0.7, 0.9};
    const Vec3D BACKGROUND_COLOR{1.,1.,1.}; // Background color (no object hit)
    const Vec3D EMPTY_COLOR{0., 0., 0.}; // No energy - black
    #define __MT__ // Multithreading
}

#endif
