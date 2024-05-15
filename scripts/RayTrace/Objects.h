#ifndef MAIN_CPP_OBJECT_H
#define MAIN_CPP_OBJECT_H

#include "../Utilities/Utils.h"
#include <vector>
#include <memory>
#include <initializer_list>
#include <algorithm>
#include <limits>

namespace RT{
    enum class ObjectType{
        BASE,
        TRIANGLE,
        TRIANGLE_MESH,
        SPHERE, // not implemented yet
        CHESSBOARD
    };

    class Object{
    public:
        Object();

        const double GetAlbedo() const;
        const Vec3D GetColor() const;

        virtual const RT::ObjectType GetType() const;
        virtual void SetCenter(const Vec3D &point);

        void SetAlbedo(double albedo);
        void SetColor(const Vec3D &color);

    private:
        Vec3D color_;
        double albedo_;
    };

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
        void SetCenter(const Vec3D &point) override;

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

    class TriangleMesh : public Object{
    public:
        TriangleMesh(std::vector<Vec3D> &vertices, std::vector<Vector<int, 3>> &triangles);

        size_t GetTriangleNum() const;

        const std::vector<Vec3D>& GetVertices() const;
        const std::vector<Vector<int, 3>>& GetTriangles() const;
        const std::vector<std::pair<Vec3D, Vec3D>>& GetEdges() const;
        const std::vector<Vec3D>& GetNormals() const;
        const std::vector<Vec3D>& GetVertexNormals() const;

        virtual const RT::ObjectType GetType() const override;
        void SetCenter(const Vec3D &point) override;
        void Fit1x1(double xOffset, double zOffset);

    private:
        void updateEdgesAndNormals();

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
