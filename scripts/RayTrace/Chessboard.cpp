#include "Chessboard.h"
#include "Material.h"
#include "../Utilities/MaterialConfig.h"

RT::Chessboard::Chessboard(const Vec3D &bottomLeft) {
    // Create chess board objects
    for (double i = 0; i < 8; ++i){
        std::vector<std::shared_ptr<RT::Object>> row;
        for (double j = 0; j < 8; ++j){
            row.push_back(nullptr);

            Vec3D pointA1 = Vec3D{i + bottomLeft[0], 0., j + bottomLeft[1]};
            Vec3D pointC1 = Vec3D{i + 1. + bottomLeft[0], 0., j + 1. + bottomLeft[1]};
            Vec3D pointB1 = Vec3D{i + bottomLeft[0], 0., j + 1. + bottomLeft[1]};
            auto pTriangle1 = std::make_shared<RT::Triangle>(pointA1, pointB1, pointC1);

            Vec3D pointA2 = Vec3D{i + 1 + bottomLeft[0], 0., j + bottomLeft[1]};
            Vec3D pointC2 = Vec3D{i + 1 + bottomLeft[0], 0., j + 1 + bottomLeft[1]};
            Vec3D pointB2 = Vec3D{i + bottomLeft[0], 0., j + bottomLeft[1]};
            auto pTriangle2 = std::make_shared<RT::Triangle>(pointA2, pointB2, pointC2);

            if ((int)(i + j) % 2 == 0){
                pTriangle1->SetMaterial(Config::WHITE_BOARD_MATERIAL);
                pTriangle2->SetMaterial(Config::WHITE_BOARD_MATERIAL);
            } else{
                pTriangle1->SetMaterial(Config::BLACK_BOARD_MATERIAL);
                pTriangle2->SetMaterial(Config::BLACK_BOARD_MATERIAL);
            }

            triangleGrid_.push_back(pTriangle1);
            triangleGrid_.push_back(pTriangle2);
        }
    }

    // Create chess pieces objects
    for (int i = 0; i < 8; ++i){
        for (int j = 0; j < 8; ++j){
            std::string figureName = figures_[i][j];
            if (figureName != "empty") {
                auto figure = ObjLoader::loadTriangleMeshObj(figureName + ".obj");
                if (figureName == "pawn"){
                    figure->Fit1x1(0.65, 0.65);
                    figure->SetMaterial(Config::PAWN_MATERIAL);
                } else if (figureName == "rook"){
                    figure->Fit1x1(0.65, 0.65);
                    figure->SetMaterial(Config::ROOK_MATERIAL);
                } else if (figureName == "bishop") {
                    figure->Fit1x1(0.75, 0.75);
                    figure->SetMaterial(Config::BISHOP_MATERIAL);
                } else if (figureName == "knight"){
                    figure->Fit1x1(0.75, 0.75);
                    figure->SetMaterial(Config::KNIGHT_MATERIAL);
                } else if (figureName == "king"){
                    figure->Fit1x1(0.8, 0.8);
                    figure->SetMaterial(Config::KING_MATERIAL);
                } else{
                    figure->Fit1x1(0.8, 0.8);
                    figure->SetMaterial(Config::QUEEN_MATERIAL);
                }
                figure->SetCenter(Vec3D{i + 0.5, 0., j + 0.5});
                triangleGrid_.push_back(figure);
            }
        }
    }

    // Create chess board rim objects
    Vec3D pointA = Vec3D{8., -1., 8.};
    Vec3D pointB = Vec3D{8., 0., 8.};
    Vec3D pointC = Vec3D{0., 0., 8.};
    auto pTriangle = std::make_shared<RT::Triangle>(pointA, pointB, pointC);
    pTriangle->SetMaterial(Config::BLACK_BOARD_MATERIAL);
    triangleGrid_.push_back(pTriangle);

    pointA = Vec3D{0., 0., 8.};
    pointB = Vec3D{0., -1., 8.};
    pointC = Vec3D{8., -1., 8.};
    pTriangle = std::make_shared<RT::Triangle>(pointA, pointB, pointC);
    pTriangle->SetMaterial(Config::BLACK_BOARD_MATERIAL);
    triangleGrid_.push_back(pTriangle);
//
//    Vec3D pointA = Vec3D{8., -1., 8.};
//    Vec3D pointB = Vec3D{8., 0., 8.};
//    Vec3D pointC = Vec3D{0., 0., 8.};
//    auto pTriangle = std::make_shared<RT::Triangle>(pointA, pointB, pointC);
//    pTriangle->SetMaterial(Config::BLACK_BOARD_MATERIAL);
//    triangleGrid_.push_back(pTriangle);
//
//    pointA = Vec3D{0., 0., 8.};
//    pointB = Vec3D{0., -1., 8.};
//    pointC = Vec3D{8., -1., 8.};
//    pTriangle = std::make_shared<RT::Triangle>(pointA, pointB, pointC);
//    pTriangle->SetMaterial(Config::BLACK_BOARD_MATERIAL);
//    triangleGrid_.push_back(pTriangle);
//
//    Vec3D pointA = Vec3D{8., -1., 8.};
//    Vec3D pointB = Vec3D{8., 0., 8.};
//    Vec3D pointC = Vec3D{0., 0., 8.};
//    auto pTriangle = std::make_shared<RT::Triangle>(pointA, pointB, pointC);
//    pTriangle->SetMaterial(Config::BLACK_BOARD_MATERIAL);
//    triangleGrid_.push_back(pTriangle);
//
//    pointA = Vec3D{0., 0., 8.};
//    pointB = Vec3D{0., -1., 8.};
//    pointC = Vec3D{8., -1., 8.};
//    pTriangle = std::make_shared<RT::Triangle>(pointA, pointB, pointC);
//    pTriangle->SetMaterial(Config::BLACK_BOARD_MATERIAL);
//    triangleGrid_.push_back(pTriangle);
//
//    Vec3D pointA = Vec3D{8., -1., 8.};
//    Vec3D pointB = Vec3D{8., 0., 8.};
//    Vec3D pointC = Vec3D{0., 0., 8.};
//    auto pTriangle = std::make_shared<RT::Triangle>(pointA, pointB, pointC);
//    pTriangle->SetMaterial(Config::BLACK_BOARD_MATERIAL);
//    triangleGrid_.push_back(pTriangle);
//
//    pointA = Vec3D{0., 0., 8.};
//    pointB = Vec3D{0., -1., 8.};
//    pointC = Vec3D{8., -1., 8.};
//    pTriangle = std::make_shared<RT::Triangle>(pointA, pointB, pointC);
//    pTriangle->SetMaterial(Config::BLACK_BOARD_MATERIAL);
//    triangleGrid_.push_back(pTriangle);
}

std::vector<std::shared_ptr<RT::Object>> RT::Chessboard::GetObjectPointers() {
    std::vector<std::shared_ptr<RT::Object>> result;
    for (auto& ptr : triangleGrid_){
        result.push_back(ptr);
    }
    for (auto& row : board_){
        for (auto& ptr : row){
            if (ptr != nullptr){
                result.push_back(ptr);
            }
        }
    }

    return result;
}
