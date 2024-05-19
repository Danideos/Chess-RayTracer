#ifndef MAIN_CPP_CHESSBOARD_H
#define MAIN_CPP_CHESSBOARD_H

#include "../Utilities/Utils.h"
#include "../ObjectLoader/ObjLoader.h"
#include "Objects.h"
#include <vector>

namespace RT{
    class Chessboard{
    public:
        Chessboard(const Vec3D& bottomLeft);

        std::vector<std::shared_ptr<RT::Object>> GetObjectPointers();

    private:
        std::vector<std::vector<std::shared_ptr<RT::Object>>> board_;
        std::vector<std::shared_ptr<RT::Object>> triangleGrid_;

        std::vector<std::vector<std::string>> figures_ = {
                {"empty", "empty", "empty", "empty", "empty", "empty", "empty", "empty"},
                {"empty", "empty", "empty", "empty", "empty", "knight", "empty", "empty"},
                {"empty", "empty", "empty", "empty", "empty", "empty", "empty", "empty"},
                {"empty", "empty", "empty", "empty", "empty", "king", "pawn", "empty"},
                {"empty", "empty", "empty", "empty", "empty", "bishop", "empty", "empty"},
                {"empty", "empty", "empty", "empty", "empty", "empty", "empty", "empty"},
                {"empty", "empty", "empty", "empty", "empty", "empty", "rook", "empty"},
                {"empty", "empty", "empty", "empty", "empty", "empty", "empty", "empty"},
        };

        // Board configuration
//    std::vector<std::vector<std::string>> figures_ = { // Fullboard configuration, not recommended for performance reasons
//        {"rook", "knight", "bishop", "queen", "king", "bishop", "knight", "rook"},
//        {"pawn", "pawn", "pawn", "pawn", "pawn", "pawn", "pawn", "pawn"},
//        {"empty", "empty", "empty", "empty", "empty", "empty", "empty", "empty"},
//        {"empty", "empty", "empty", "empty", "empty", "empty", "empty", "empty"},
//        {"empty", "empty", "empty", "empty", "empty", "empty", "empty", "empty"},
//        {"empty", "empty", "empty", "empty", "empty", "empty", "empty", "empty"},
//        {"pawn", "pawn", "pawn", "pawn", "pawn", "pawn", "pawn", "pawn"},
//        {"rook", "knight", "bishop", "queen", "king", "bishop", "knight", "rook"}
//    };
    };
}

#endif

