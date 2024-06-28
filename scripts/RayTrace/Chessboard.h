/**
 * @file Chessboard.h
 * @brief Defines the Chessboard class used for ray tracing.
 */
#ifndef MAIN_CPP_CHESSBOARD_H
#define MAIN_CPP_CHESSBOARD_H

#include "../Utilities/Utils.h"
#include "../ObjectLoader/ObjLoader.h"
#include "Material.h"
#include "../Utilities/Config.h"
#include "Objects.h"
#include <vector>

namespace RT{
    class Chessboard{
    public:
        /** \brief Creates the chessboard object and its pieces
         *
         * @param bottomLeft sets up the bottom left corner coordinates of the chessboard
         */
        Chessboard(const Vec3D& bottomLeft);
        /** \brief Returns pointers to stored objects(board, pieces)
         *
         *
         */
        std::vector<std::shared_ptr<RT::Object>> GetObjectPointers();

    private:
        std::vector<std::vector<std::shared_ptr<RT::Object>>> board_;
        std::vector<std::shared_ptr<RT::Object>> triangleGrid_;

        std::vector<std::vector<std::string>> figures_ = Config::FIGURE_CONFIGURATION;
    };
}

#endif

