#ifndef MAIN_CPP_CHESSBOARD_H
#define MAIN_CPP_CHESSBOARD_H

#include "../Utilities/Utils.h"
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
    };
}

#endif

