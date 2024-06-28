#include "Objects.h"
#include <float.h>

// Base
RT::Object::Object() {
    pMaterial_ = nullptr;
}

const RT::ObjectType RT::Object::GetType() const {
    return RT::ObjectType::BASE;
}

// Base class skips implementation...
void RT::Object::SetCenter(const Vec3D point) {}

std::pair<Vec3D, Vec3D > RT::Object::GetBoundingPoints() const {
    return std::pair<Vec3D, Vec3D>(std::make_pair<Vec3D, Vec3D>(Vec3D{0., 0., 0.}, Vec3D{0., 0., 0.}));
}

void RT::Object::SetPos(const Vec3D point) {}


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

void RT::Triangle::SetCenter(const Vec3D point) {
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

std::pair<Vec3D, Vec3D> RT::Triangle::GetBoundingPoints() const {
    Vec3D minPoint = Vec3D{
        std::min(std::min(pointA_[0], pointB_[0]), pointC_[0]),
        std::min(std::min(pointA_[1], pointB_[1]), pointC_[1]),
        std::min(std::min(pointA_[2], pointB_[2]), pointC_[2])
    };
    Vec3D maxPoint = Vec3D{
        std::max(std::max(pointA_[0], pointB_[0]), pointC_[0]),
        std::max(std::max(pointA_[1], pointB_[1]), pointC_[1]),
        std::max(std::max(pointA_[2], pointB_[2]), pointC_[2])
    };
    return std::make_pair(minPoint, maxPoint);
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

void RT::TriangleMesh::SetCenter(const Vec3D point) {
    Vec3D center{0., 0., 0.};
    double minY = DBL_MAX;
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
    std::vector<int> facesCounter;
    std::vector<Vec3D> vertexNormals(vertices_.size());
    for (int i = 0; i < vertices_.size(); ++i){
        facesCounter.emplace_back(0);
        vertexNormals.emplace_back(Vec3D{0., 0., 0.});
    }

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

        vertexNormals[triangle[0]] = vertexNormals[triangle[0]] + normal;
        vertexNormals[triangle[1]] = vertexNormals[triangle[1]] + normal;
        vertexNormals[triangle[2]] = vertexNormals[triangle[2]] + normal;
        facesCounter[triangle[0]]++;
        facesCounter[triangle[1]]++;
        facesCounter[triangle[2]]++;
    }
    for (int i = 0; i < vertices_.size(); ++i) {
        if (facesCounter[i] > 0){
            vertexNormals[i] = (vertexNormals[i] / facesCounter[i]);
        } else{
            vertexNormals[i].normalize();
        }
    }
    vertexNormals_ = vertexNormals;
}

const std::vector<Vec3D > &RT::TriangleMesh::GetVertexNormals() const {
    return vertexNormals_;
}

std::pair<Vec3D, Vec3D > RT::TriangleMesh::GetBoundingPoints() const {
    Vec3D pointA = vertices_[triangles_[0][0]];
    Vec3D pointB = vertices_[triangles_[0][1]];
    Vec3D pointC = vertices_[triangles_[0][2]];
    Vec3D minPoint = Vec3D{
            std::min(std::min(pointA[0], pointB[0]), pointC[0]),
            std::min(std::min(pointA[1], pointB[1]), pointC[1]),
            std::min(std::min(pointA[2], pointB[2]), pointC[2])
    };
    Vec3D maxPoint = Vec3D{
            std::max(std::max(pointA[0], pointB[0]), pointC[0]),
            std::max(std::max(pointA[1], pointB[1]), pointC[1]),
            std::max(std::max(pointA[2], pointB[2]), pointC[2])
    };
    for (auto triangle : triangles_){
        minPoint = Vec3D{
                std::min(std::min(std::min(vertices_[triangle[0]][0], vertices_[triangle[0]][0]), vertices_[triangle[0]][0]), minPoint[0]),
                std::min(std::min(std::min(vertices_[triangle[1]][1], vertices_[triangle[1]][1]), vertices_[triangle[1]][1]), minPoint[1]),
                std::min(std::min(std::min(vertices_[triangle[2]][2], vertices_[triangle[2]][2]), vertices_[triangle[2]][2]), minPoint[2])
        };
        maxPoint = Vec3D{
                std::max(std::max(std::max(vertices_[triangle[0]][0], vertices_[triangle[0]][0]), vertices_[triangle[0]][0]), maxPoint[0]),
                std::max(std::max(std::max(vertices_[triangle[1]][1], vertices_[triangle[1]][1]), vertices_[triangle[1]][1]), maxPoint[1]),
                std::max(std::max(std::max(vertices_[triangle[2]][2], vertices_[triangle[2]][2]), vertices_[triangle[2]][2]), maxPoint[2])
        };
    }
    return std::make_pair(minPoint, maxPoint);
}

void RT::TriangleMesh::SetPos(const Vec3D point) {
    Vec3D leftCorner = vertices_[0];
    for (auto& vertex : vertices_){
        leftCorner[0] = std::min(leftCorner[0], vertex[0]);
        leftCorner[1] = std::min(leftCorner[1], vertex[1]);
        leftCorner[2] = std::min(leftCorner[2], vertex[2]);
    }
    Vec3D diff = leftCorner - point;
    for (auto& vertex : vertices_){
        vertex = vertex - diff;
    }
}

void RT::TriangleMesh::Rotate(double angle) {
    Vec3D center{0., 0., 0.};
    double minY = DBL_MAX;
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
    double angleRad = angle * (M_PI / 180.);
    for (auto& vertex : vertices_){
        RotatePointAroundCenter(vertex, center, angleRad);
    }
    updateEdgesAndNormals();
}

void RT::TriangleMesh::RotatePointAroundCenter(Vec3D& point, Vec3D center, double angle) {
    double s = sin(angle);
    double c = cos(angle);
    point[0] -= center[0];
    point[2] -= center[2];
    double newX = point[0] * c - point[2] * s;
    double newY = point[0] * s + point[2] * c;
    point[0] = newX + center[0];
    point[2] = newY + center[2];
}


