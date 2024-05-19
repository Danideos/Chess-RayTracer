#ifndef MAIN_CPP_CONFIG_H
#define MAIN_CPP_CONFIG_H

#include "../RayTrace/Material.h"
#include "Utils.h"

namespace Config
{
    // Example of materials
//    auto black_lambertian = std::make_shared<RT::Lambertian>(RT::Lambertian(Utils::BLACK_BOARD_COLOR)); // Color
//    auto white_lambertian = std::make_shared<RT::Lambertian>(RT::Lambertian(Utils::WHITE_BOARD_COLOR));
//    auto black_metal = std::make_shared<RT::Metal>(RT::Metal(Utils::BLACK_PIECE_COLOR, 0.01)); // Color, Fuzz
//    auto white_metal = std::make_shared<RT::Metal>(RT::Metal(Utils::WHITE_PIECE_COLOR, 0.01));
//    auto white_dielectric = std::make_shared<RT::Dielectric>(1., Vec3D{1., 1., 1.}, Vec3D{0.6, 1., 0.6}); // Smoothness, Transparency, Color
//    auto black_dielectric = std::make_shared<RT::Dielectric>(1., Vec3D{1., 1., 1.}, Vec3D{0.7, 0.7, 1.});

    // Pieces material config
    const auto KNIGHT_MATERIAL = std::make_shared<RT::Lambertian>(Utils::WHITE_PIECE_COLOR);
    const auto PAWN_MATERIAL = std::make_shared<RT::Lambertian>(Utils::BLACK_PIECE_COLOR);
    const auto QUEEN_MATERIAL = std::make_shared<RT::Dielectric>(1., Vec3D{1., 1., 1.}, Vec3D{0.6, 1., 0.6});
    const auto KING_MATERIAL = std::make_shared<RT::Dielectric>(1., Vec3D{1., 1., 1.}, Vec3D{0.6, 1., 0.6});
    const auto BISHOP_MATERIAL = std::make_shared<RT::Dielectric>(1., Vec3D{1., 1., 1.}, Vec3D{0.7, 0.7, 1.});
    const auto ROOK_MATERIAL = std::make_shared<RT::Metal>(RT::Metal(Utils::BLACK_PIECE_COLOR, 0.01));
    const auto BLACK_BOARD_MATERIAL = std::make_shared<RT::Lambertian>(RT::Lambertian(Utils::BLACK_BOARD_COLOR));
    const auto WHITE_BOARD_MATERIAL = std::make_shared<RT::Lambertian>(RT::Lambertian(Utils::WHITE_BOARD_COLOR));
}

#endif
