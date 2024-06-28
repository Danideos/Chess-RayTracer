#include "../Random/Random.h"
#include "PDF.h"

RT::CosinePDF::CosinePDF(Vec3D hitNormal) {
    hitNormal.normalize();
    Vec3D a = (abs(hitNormal[0]) > 0.9) ? Vec3D{0., 1., 0.} : Vec3D{1., 0., 0.};
    Vec3D v = cross(hitNormal, a).normalized();
    Vec3D u = cross(hitNormal, v);
    axis_.emplace_back(u);
    axis_.emplace_back(v);
    axis_.emplace_back(hitNormal);
}

Vec3D RT::CosinePDF::RandomCosineDirection() const {
    auto random1 = Rand::RandomDouble();
    auto random2 = Rand::RandomDouble();
    auto a = 2 * M_PI  * random1;
    return Vec3D{cos(a) * sqrt(random2), sin(a) * sqrt(random2), sqrt(1 - random2)};
}

Vec3D RT::CosinePDF::Local(Vec3D a) const {
    return a[0] * axis_[0] + a[1] * axis_[1] + a[2] * axis_[2];
}

double RT::CosinePDF::Value(Vec3D direction) const {
    auto cosineTheta = dot(direction.normalized(), axis_[2]);
    return std::max(0., cosineTheta / M_PI);
}

Vec3D RT::CosinePDF::Generate() const {
    return Local(RandomCosineDirection());
}