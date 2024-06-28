/**
 * @file Objects.h
 * @brief Defines various object classes used for ray tracing.
 */

#ifndef MAIN_CPP_OBJECT_H
#define MAIN_CPP_OBJECT_H

#include "../Utilities/Utils.h"
#include <vector>
#include <memory>
#include <initializer_list>
#include <algorithm>
#include <limits>

namespace RT{

    class Material;
    /**
     * @enum ObjectType
     * @brief Enum representing the type of an object.
     */
    enum class ObjectType{
        BASE,
        TRIANGLE,
        TRIANGLE_MESH,
        SPHERE // not implemented yet
    };
    /**
     * @class Object
     * @brief Abstract base class for all objects in the ray tracing environment.
     */
    class Object{
    public:
        Object();

        virtual const RT::ObjectType GetType() const;
        virtual void SetCenter(const Vec3D point);
        virtual void SetPos(const Vec3D point);

        void SetMaterial(std::shared_ptr<RT::Material> pMaterial) { pMaterial_ = pMaterial; }
        std::shared_ptr<RT::Material> GetMaterial() { return pMaterial_; };
        /**
         * @brief Gets the bounding points of the object.
         * @return A pair of vectors representing the bounding points.
         */
        virtual std::pair<Vec3D, Vec3D> GetBoundingPoints() const;

    private:
        std::shared_ptr<RT::Material> pMaterial_;
    };

    /**
     * @class Triangle
     * @brief Represents a triangular object in the ray tracing environment.
     */
    class Triangle : public Object{
    public:
        Triangle(const Vec3D &pointA, const Vec3D &pointB, const Vec3D &pointC);

        const Vec3D& GetPointA() const;
        const Vec3D& GetPointB() const;
        const Vec3D& GetPointC() const;
        const Vec3D& GetEdgeAB() const;
        const Vec3D& GetEdgeAC() const;
        const Vec3D& GetNormal() const;

        virtual const RT::ObjectType GetType() const override;
        void SetCenter(const Vec3D point) override;
        virtual std::pair<Vec3D, Vec3D> GetBoundingPoints() const override;

    private:
        // Important to define points counterclockwise for cross product to return correct orientation
        Vec3D pointA_;
        Vec3D pointB_;
        Vec3D pointC_;
        // Params, which we use in intersection testing
        Vec3D triangleNormal_;
        Vec3D edgeAB_;
        Vec3D edgeAC_;
        Vec3D edgeBC_;
    };
    /**
     * @class TriangleMesh
     * @brief Represents a mesh of triangles for effective representation in the ray tracing environment.
     */
    class TriangleMesh : public Object{
    public:
        TriangleMesh(std::vector<Vec3D> &vertices, std::vector<Vector<int, 3>> &triangles);

        size_t GetTriangleNum() const;

        const std::vector<Vec3D>& GetVertices() const;
        const std::vector<Vector<int, 3>>& GetTriangles() const;
        const std::vector<std::pair<Vec3D, Vec3D>>& GetEdges() const;
        const std::vector<Vec3D>& GetNormals() const;
        const std::vector<Vec3D>& GetVertexNormals() const;
        void SetSmoothness(double smoothness) { smoothness_ = smoothness; };
        double GetSmoothness() { return smoothness_; };

        virtual const RT::ObjectType GetType() const override;
        virtual std::pair<Vec3D, Vec3D> GetBoundingPoints() const override;
        /// \brief Set center coordinates of the object
        void SetCenter(const Vec3D point) override;
        /// \brief Set bottom left coordinates of the object(min x, min y, min z)
        void SetPos(const Vec3D point) override;
        /// \brief Rescale the object to fit 1x1 square(x and z coordinates) with x and y multiplier to 1x1 square
        void Fit1x1(double xOffset, double zOffset);
        /// \brief Recalculate edges and normals after changing object
        void updateEdgesAndNormals();
        /**
         * @brief Rotates the mesh around its center.
         * @param angle The angle in degrees to rotate the mesh.
         */
        void Rotate(double angle);

    private:
        void RotatePointAroundCenter(Vec3D& point, Vec3D center, double angleInRadians);

        double smoothness_ = Utils::BASE_SMOOTHNESS;

        // Intersection testing first done on bounding volumes to save computation time
        std::vector<RT::Triangle*> boundingVolume_;

        // Triangle mesh base params
        std::vector<Vec3D> vertices_;
        std::vector<Vector<int, 3>> triangles_;

        // Precalculated params needed for intersection testing
        std::vector<Vec3D> normals_;
        std::vector<Vec3D> vertexNormals_;
        std::vector<std::pair<Vec3D, Vec3D>> edges_; // only two edges needed for our intersection testing
    };
    /**
     * @class DistantLightSource
     * @brief Represents a distant light source in the ray tracing environment.
     */
    class DistantLightSource{
    public:
        DistantLightSource();

        const Vec3D GetColor() const;
        const Vec3D GetDirection() const;
        const double GetIntensity() const;

        void SetColor(const Vec3D &color);
        void SetDirection(Vec3D &direction);
        void SetIntensity(double intensity);

    private:
        Vec3D color_;
        Vec3D direction_;
        double intensity_;
    };

}

#endif
