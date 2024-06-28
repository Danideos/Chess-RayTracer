/**
 * @file Camera.h
 * @brief Defines the Camera class used for ray tracing.
 */
#ifndef MAIN_CPP_CAMERA_H
#define MAIN_CPP_CAMERA_H

#include "../LinearAlgebra/Vector.h"
#include "../Utilities/Utils.h"
#include "Ray.h"

namespace RT{
    /**
     * @class Camera
     * @brief Represents a camera in the ray tracing environment.
     *
     * The Camera class is responsible for setting up the camera's position,
     * orientation, and field of view for rendering scenes in the ray tracing
     * application.
     */
    class Camera{
    public:
        Camera();
        void SetPos(Vec3D pos);
        void SetLookAt(Vec3D lookAt);
        void SetUp(Vec3D up);
        void SetLookLength(double lookLength);
        void SetAspectRatio(double aspectRatio);
        void SetHorizontalScale(double horizontalScale);

        Vec3D GetPos() { return cameraPos_; }
        Vec3D GetCenter() { return screenCenter_; }
        Vec3D GetScreenU() { return screenU_; }
        Vec3D GetScreenV() { return screenV_; }
        /// \brief Calculates additional camera parameters from the provided ones
        void CalculateParams();
        /**
         * @brief Generates a ray passing through a specified point on the screen.
         *
         * This method calculates the ray that starts at the camera's position
         * and passes through the point (u, v) on the screen.
         *
         * @param u The horizontal coordinate on the screen, in the range [0, 1].
         * @param v The vertical coordinate on the screen,d in the range [0, 1].
         * @return The ray passing through the point (u, v).
         */
        RT::Ray GetRay(double xNorm, double yNorm);
    private:
        // these 3 vectors fully determine how camera is positioned in the world
        Vec3D cameraPos_;
        Vec3D cameraLookAt_;
        Vec3D cameraUp_;

        // represents screen coordinate system: up, right and center
        Vec3D screenU_;
        Vec3D screenV_;
        Vec3D screenCenter_;

        // parameters to transfer screen coordinate system to world coordinates
        double lookLength_;
        double aspectRatio_;
        double horizontalScale_;
    };
}

#endif
