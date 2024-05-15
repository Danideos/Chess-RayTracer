#include "Chessboard.h"

RT::Chessboard::Chessboard(const Vector<double, DIMS_3D> &bottomLeft) {
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

            if (int(i + j) % 2 == 1){
                pTriangle1->SetColor(Utils::WHITE_BOARD_COLOR);
                pTriangle2->SetColor(Utils::WHITE_BOARD_COLOR);
            } else{
                pTriangle1->SetColor(Utils::BLACK_BOARD_COLOR);
                pTriangle2->SetColor(Utils::BLACK_BOARD_COLOR);
            }

            triangleGrid_.push_back(pTriangle1);
            triangleGrid_.push_back(pTriangle2);
        }
    }
    Vec3D pointA = Vec3D{8., -1., 8.};
    Vec3D pointB = Vec3D{8., 0., 8.};
    Vec3D pointC = Vec3D{0., 0., 8.};
    auto pTriangle = std::make_shared<RT::Triangle>(pointA, pointB, pointC);
    pTriangle->SetColor(Utils::BLACK_BOARD_COLOR);
    triangleGrid_.push_back(pTriangle);

    pointA = Vec3D{0., 0., 8.};
    pointB = Vec3D{0., -1., 8.};
    pointC = Vec3D{8., -1., 8.};
    pTriangle = std::make_shared<RT::Triangle>(pointA, pointB, pointC);
    pTriangle->SetColor(Utils::BLACK_BOARD_COLOR);
    triangleGrid_.push_back(pTriangle);
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
