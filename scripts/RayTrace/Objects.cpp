#include "Objects.h"

// Base
RT::Object::Object() {
    color_ = Utils::BASE_COLOR;
    albedo_ = Utils::BASE_ALBEDO;
}

const double RT::Object::GetAlbedo() const {
    return albedo_;
}

const Vec3D RT::Object::GetColor() const {
    return color_;
}

void RT::Object::SetAlbedo(double albedo) {
    albedo_ = albedo;
}

void RT::Object::SetColor(const Vec3D &color) {
    color_ = color;
}

const RT::ObjectType RT::Object::GetType() const {
    return RT::ObjectType::BASE;
}

void RT::Object::SetCenter(const Vec3D &point) {} // Base class skips implementation...

// Triangle
RT::Triangle::Triangle(const Vec3D &pointA, const Vec3D &pointB, const Vec3D &pointC)
        : pointA_(pointA), pointB_(pointB), pointC_(pointC) {

    edgeAB_ = pointB_ - pointA_;
    edgeBC_ = pointC_ - pointB_;
    edgeAC_ = pointC_ - pointA_;

    triangleNormal_ = cross(edgeAB_, edgeAC_);
    triangleNormal_.normalize();
}

const Vec3D& RT::Triangle::GetPointA() const {
    return pointA_;
}

const Vec3D& RT::Triangle::GetPointB() const {
    return pointB_;
}

const Vec3D& RT::Triangle::GetPointC() const {
    return pointC_;
}

const Vec3D& RT::Triangle::GetEdgeAB() const {
    return edgeAB_;
}

const Vec3D& RT::Triangle::GetEdgeAC() const {
    return edgeAC_;
}

const Vec3D& RT::Triangle::GetNormal() const {
    return triangleNormal_;
}

const RT::ObjectType RT::Triangle::GetType() const {
    return RT::ObjectType::TRIANGLE;
}

void RT::Triangle::SetCenter(const Vec3D &point) {
    Vec3D bottomLeftCorner{
        std::min(std::min(pointA_[0], pointB_[0]), pointC_[0]),
        std::min(std::min(pointA_[1], pointB_[1]), pointC_[1]),
        std::min(std::min(pointA_[2], pointB_[2]), pointC_[2])
    };
    Vec3D diff = bottomLeftCorner - point;
    pointA_ = pointA_ - diff;
    pointB_ = pointB_ - diff;
    pointC_ = pointC_ - diff;
}


// Light
RT::DistantLightSource::DistantLightSource() {
    color_ = Utils::LIGHT_COLOR;
    direction_ = Utils::LIGHT_DIR.normalized();
    intensity_ = Utils::LIGHT_INTENSITY;
}

const Vec3D RT::DistantLightSource::GetColor() const {
    return color_;
}


const Vec3D RT::DistantLightSource::GetDirection() const {
    return direction_;
}

const double RT::DistantLightSource::GetIntensity() const {
    return intensity_;
}

void RT::DistantLightSource::SetColor(const Vec3D &color) {
    color_ = color;
}

void RT::DistantLightSource::SetDirection(Vec3D &direction) {
    direction_ = direction;
}

void RT::DistantLightSource::SetIntensity(double intensity) {
    intensity_ = intensity;
}

// Triangle mesh
RT::TriangleMesh::TriangleMesh(std::vector<Vec3D> &vertices, std::vector<Vector<int, 3>> &triangles){
    vertices_ = vertices;
    triangles_ = triangles;

    updateEdgesAndNormals();

    // Calculate bounding volume triangles
    // - missing
}

size_t RT::TriangleMesh::GetTriangleNum() const {
    return triangles_.size();
}

const std::vector<Vec3D>& RT::TriangleMesh::GetVertices() const {
    return vertices_;
}

const std::vector<Vector<int, 3>>& RT::TriangleMesh::GetTriangles() const {
    return triangles_;
}

const std::vector<std::pair<Vec3D, Vec3D>>& RT::TriangleMesh::GetEdges() const {
    return edges_;
}

const std::vector<Vec3D>& RT::TriangleMesh::GetNormals() const {
    return normals_;
}

const RT::ObjectType RT::TriangleMesh::GetType() const {
    return RT::ObjectType::TRIANGLE_MESH;
}

void RT::TriangleMesh::SetCenter(const Vec3D &point) {
    Vec3D center{0., 0., 0.};
    double minY;
    for (auto& vertex : vertices_){
        minY = std::min(minY, vertex[1]);
    }
    int amount = 0;
    for (auto& vertex : vertices_){
        if (minY == vertex[1]){
            center = center + vertex;
            amount++;
        }
    }
    center = center / amount;
    Vec3D diff = center - point;
    for (auto& vertex : vertices_){
        vertex = vertex - diff;
    }
}

void RT::TriangleMesh::Fit1x1(double xOffset, double zOffset) {
    Vec3D bottomLeftCorner{
            std::numeric_limits<double>::max(),
            std::numeric_limits<double>::max(),
            std::numeric_limits<double>::max()
    };
    Vec3D topRightCorner{
            std::numeric_limits<double>::min(),
            std::numeric_limits<double>::max(),
            std::numeric_limits<double>::min(),
    };
    for (auto& vertex : vertices_){
        bottomLeftCorner[0] = std::min(bottomLeftCorner[0], vertex[0]);
        bottomLeftCorner[1] = std::min(bottomLeftCorner[1], vertex[1]);
        bottomLeftCorner[2] = std::min(bottomLeftCorner[2], vertex[2]);

        topRightCorner[0] = std::max(topRightCorner[0], vertex[0]);
        topRightCorner[1] = std::min(topRightCorner[1], vertex[1]);
        topRightCorner[2] = std::max(topRightCorner[2], vertex[2]);
    }
    double xScale = topRightCorner[0] - bottomLeftCorner[0];
    double zScale = topRightCorner[2] - bottomLeftCorner[2];
    for (auto& vertex : vertices_){
        vertex[0] = (vertex[0] - bottomLeftCorner[0]) / xScale * xOffset;
        vertex[1] = (vertex[1] - bottomLeftCorner[1]) / xScale * xOffset;
        vertex[2] = (vertex[2] - bottomLeftCorner[2]) / zScale * zOffset;

    }
    updateEdgesAndNormals();
}

void RT::TriangleMesh::updateEdgesAndNormals() {
    edges_.clear();
    normals_.clear();
    for (auto& triangle : triangles_){
        Vec3D pointA = vertices_[triangle[0]];
        Vec3D pointB = vertices_[triangle[1]];
        Vec3D pointC = vertices_[triangle[2]];

        Vec3D edgeAB = pointB - pointA;
        Vec3D edgeAC = pointC - pointA;
        std::pair<Vec3D, Vec3D> edge{edgeAB, edgeAC};

        Vec3D normal = cross(edgeAB, edgeAC);
        normal.normalize();

        edges_.emplace_back(edge);
        normals_.emplace_back(normal);
    }
}


