/**
 * @file Ray.h
 * @brief Defines the Ray class and associated structures used for ray tracing.
 */

#ifndef MAIN_CPP_RAY_H
#define MAIN_CPP_RAY_H

#include "../LinearAlgebra/Vector.h"
#include "../Utilities/Utils.h"
#include "Objects.h"
#include <float.h>

namespace RT{
    /**
     * @struct HitPayload
     * @brief Stores data related to a ray-object intersection.
     */
    struct HitPayload{ // Desired return params of ray intersection test
        double u;
        double v;
        double hitDist = DBL_MAX;
        Vec3D hitPoint;
        Vec3D hitNormal;
        bool frontFace;
        std::shared_ptr<RT::Object> pObject = nullptr;
    };
    /**
     * @class Ray
     * @brief Represents a ray in the ray tracing environment.
     */
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
        /// \brief Get reflected ray direction along a hit surface normal
        const Vec3D GetReflected(const Vec3D& reflectNormal) const;
        /// \brief Get retracted ray direction along a hit surface normal of dielectric material
        const Vec3D GetRefracted(const Vec3D& refractNormal, double ri) const;
        /// \brief Check if ray intersects with object
        void RayIntersect(std::shared_ptr<RT::Object> pObject, RT::HitPayload& payload);

        /// \brief Reflect this ray along a hit surface normal
        void Reflect(const Vec3D &reflectNormal, const Vec3D &rayStart);
    private:
        RT::HitPayload RayTriangleIntersect(const Vec3D &pointA, const Vec3D &pointB, const Vec3D &pointC,
                                            const Vec3D &edgeAB, const Vec3D &edgeAC, const Vec3D &faceNormal, double smoothness,
                                            const Vec3D &hitNormal1, const Vec3D &hitNormal2, const Vec3D &hitNormal3);
        RT::HitPayload RayTriangleMeshIntersect(RT::TriangleMesh* pTriangleMesh);
        Vec3D startPoint_;
        Vec3D screenPoint_;
        Vec3D direction_;
    };
}

#endif
