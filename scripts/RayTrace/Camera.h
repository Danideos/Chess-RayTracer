#ifndef MAIN_CPP_CAMERA_H
#define MAIN_CPP_CAMERA_H

#include "../LinearAlgebra/Vector.h"
#include "../Utilities/Utils.h"
#include "Ray.h"

namespace RT{
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

        void CalculateParams();

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
