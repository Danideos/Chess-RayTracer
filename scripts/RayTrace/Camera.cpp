#include "Camera.h"

RT::Camera::Camera() {
    cameraPos_ = Utils::DEFAULT_CAMERA_POS;
    cameraLookAt_ = Utils::DEFAULT_CAMERA_LOOKAT;
    cameraUp_ = Utils::DEFAULT_CAMERA_UP;
    lookLength_ = Utils::DEFAULT_CAMERA_LENGTH;
    horizontalScale_ = Utils::DEFAULT_CAMERA_HORSCALE;
    aspectRatio_ = Utils::DEFAULT_CAMERA_ASPECT;

    CalculateParams();
}

void RT::Camera::SetPos(Vec3D pos) {
    cameraPos_ = pos;
}

void RT::Camera::SetLookAt(Vec3D lookAt) {
    cameraLookAt_ = lookAt;
}

void RT::Camera::SetUp(Vec3D up) {
    cameraUp_ = up;
}

void RT::Camera::SetLookLength(double lookLength) {
    lookLength_ = lookLength;
}

void RT::Camera::SetAspectRatio(double aspectRatio) {
    aspectRatio_ = aspectRatio;
}

void RT::Camera::SetHorizontalScale(double horizontalScale) {
    horizontalScale_ = horizontalScale;
}

void RT::Camera::CalculateParams() {
    Vec3D lookDir = cameraLookAt_ - cameraPos_;
    lookDir.normalize();
    screenU_ = cross(lookDir, cameraUp_);
    Vec3D zeroVec;
    if (screenU_ == zeroVec){
        throw std::invalid_argument("Camera up is the same as look direction, can't determine screen up");
    }
    screenU_.normalize();

    screenV_ = cross(screenU_, lookDir);
    screenV_.normalize();

    screenU_ = screenU_ * horizontalScale_;
    screenV_ = screenV_ * (horizontalScale_ / aspectRatio_);

    screenCenter_ = cameraPos_ + (lookDir * lookLength_);
}

RT::Ray RT::Camera::GetRay(double xNorm, double yNorm) {
    Vec3D screenPoint = screenCenter_ + (screenU_ * xNorm) + (screenV_ * yNorm);
    RT::Ray ray(cameraPos_, screenPoint);
    return ray;
}

Vec3D RT::Camera::GetPos() {
    return cameraPos_;
}
