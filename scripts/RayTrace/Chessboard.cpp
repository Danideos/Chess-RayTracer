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
                pTriangle1->SetColor(Vec3D{0.5, .2 , 0.2});
                pTriangle2->SetColor(Vec3D{0.5, .2 , 0.2});
            } else{
                pTriangle1->SetColor(Vec3D{1., 0.8 , .65});
                pTriangle2->SetColor(Vec3D{1., 0.8 , .65});
            }

            triangleGrid_.push_back(pTriangle1);
            triangleGrid_.push_back(pTriangle2);
        }
    }
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
