#ifndef MAIN_CPP_UTILS_H
#define MAIN_CPP_UTILS_H

#include "../LinearAlgebra/Vector.h"

namespace Utils{
    /** @file */
    /**
     * @{ \name RayTrace params
     */
    /// \brief Amount of ray bounces in the scene
    constexpr const int BOUNCES = 20;
    /// \brief Squared amount of rays shot into scene for each pixel
    constexpr const int SQRT_SAMPLES = 3;
    /**
     * @}
     */

    /**
     * @{ \name Ray params
     */
    /// \brief Ray parallel detection precision
    constexpr const double PARALLEL_PRECISION = 1e-3;
    /**
     * @}
     */

    /**
     * @{ \name Light source params
     */
     /// \brief Light intensity
    constexpr const double LIGHT_INTENSITY = 15.;
    /// \brief Light color
    const Vec3D LIGHT_COLOR = Vec3D{1., 1., 1.};
    /// \brief Light direction
    const Vec3D LIGHT_DIR = Vec3D{-1, -1., -1.};
    /**
     * @}
     */

    /**
     * @{ \name Camera params
     */
    /// \brief Default camera position
    const Vec3D DEFAULT_CAMERA_POS{8., 8., 16.};
    /// \brief Point at which the camera looks at, center of the screen
    const Vec3D DEFAULT_CAMERA_LOOKAT{4., 0., 4.};
    /// \brief Fixates the up vector of the screen
    const Vec3D DEFAULT_CAMERA_UP{0., 1., 0.};
    /// \brief Distance of screen from camera positition
    constexpr const double DEFAULT_CAMERA_LENGTH = 1.;
    /// \brief Aspect ratio of screen width to height
    constexpr const double DEFAULT_CAMERA_ASPECT = 16. / 9. ; // Aspect ratio of screen width to height
    /// \brief Horizontal scale of screen width
    constexpr const double DEFAULT_CAMERA_HORSCALE = 1. ; // Horizontal scale of screen width
    /**
     * @}
     */


    /** \brief Allows for smooth triangle meshes(uses barycentric coordinates),
    * (note: dielectric surfaces work well only with smoothing)
     */
    #define __SMOOTHING__
    /**
     * @{ \name Object params
     */
    /// \brief Unassigned color base
    const Vec3D BASE_ALBEDO = Vec3D{0.7, 0.7, 0.7};
    /// \brief Unassigned base refraction index of dielectric surfaces, glass has ~1.5
    constexpr const double BASE_REFRACTION_INDEX = 1.5;
    /// \brief Unassigned base RGB transparency of dielectric surfaces
    const Vec3D BASE_TRANSPARENCY = Vec3D{1., 1., 1.};
    /// \brief Unassigned base smoothness of all objects, range in [0, 1]
    constexpr const double BASE_SMOOTHNESS = 1.;
    /// \brief Unassigned base fuzziness of metal surfaces
    constexpr const double BASE_FUZZ = 0.;
    /**
     * @}
     */

    /**
     * @{ \name Color constants
     */
     /// \brief RGB max
    constexpr const double RGB_MAX = 255.;
    /**
     * @}
     */

    /**
     * @{ \name Color params
     */
    /// \brief Chessboard color of light squares
    const Vec3D WHITE_BOARD_COLOR{1., 0.8 , .65};
    /// \brief Chessboard color of dark squares
    const Vec3D BLACK_BOARD_COLOR{0.5, .2 , 0.2};
    /// \brief Object color of white pieces
    const Vec3D WHITE_PIECE_COLOR{1., 0.8 , 0.65};
    /// \brief Object color of dark pieces
    const Vec3D BLACK_PIECE_COLOR{0.5, .2 , 0.2};
    /// \brief Base color when ray doesn't hit any object in scene
    const Vec3D BACKGROUND_COLOR{1.,1.,1.};
    /// \brief No energy, black
    const Vec3D EMPTY_COLOR{0., 0., 0.};
    /**
     * @}
     */
    /// \brief Multithreading
    #define __MT__
    /** @} */
}

#endif
