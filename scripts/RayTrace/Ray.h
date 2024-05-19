#ifndef MAIN_CPP_RAY_H
#define MAIN_CPP_RAY_H

#include "../LinearAlgebra/Vector.h"
#include "../Utilities/Utils.h"
#include "Objects.h"
#include <float.h>

namespace RT{

    struct HitPayload{ // Desired return params of ray intersection test
        double u;
        double v;
        double hitDist = DBL_MAX;
        Vec3D hitPoint;
        Vec3D hitNormal;
        bool frontFace;
        std::shared_ptr<RT::Object> pObject = nullptr;
    };

    class Ray{
    public:
        Ray();
        Ray(const Vec3D& startPoint, const Vec3D& screenPoint);

        void SetStartPoint(Vec3D &startPoint);
        void SetScreenPoint(Vec3D &screenPoint);
        void SetDirection(Vec3D &direction);

        const Vec3D GetStartPoint() const;
        const Vec3D GetScreenPoint() const;
        const Vec3D GetDirection() const;
        const Vec3D GetReflected(const Vec3D& reflectNormal) const;
        const Vec3D GetRefracted(const Vec3D& refractNormal, double ri) const;

        void RayIntersect(std::shared_ptr<RT::Object> pObject, RT::HitPayload& payload);

        RT::HitPayload RayTriangleIntersect(const Vec3D &pointA, const Vec3D &pointB, const Vec3D &pointC,
                                            const Vec3D &edgeAB, const Vec3D &edgeAC, const Vec3D &faceNormal, double smoothness,
                                            const Vec3D &hitNormal1, const Vec3D &hitNormal2, const Vec3D &hitNormal3);
        RT::HitPayload RayTriangleMeshIntersect(RT::TriangleMesh* pTriangleMesh);
        void Reflect(const Vec3D &reflectNormal, const Vec3D &rayStart);
    private:
        Vec3D startPoint_;
        Vec3D screenPoint_;
        Vec3D direction_;
    };
}

#endif
