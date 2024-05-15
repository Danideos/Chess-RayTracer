#include "Ray.h"

RT::Ray::Ray() {}

RT::Ray::Ray(const Vec3D& startPoint, const Vec3D& screenPoint) {
    startPoint_ = startPoint;
    screenPoint_ = screenPoint;
    direction_ = screenPoint - startPoint;
    direction_.normalize();
}

const Vec3D RT::Ray::GetStartPoint() const {
    return startPoint_;
}

const Vec3D RT::Ray::GetScreenPoint() const {
    return screenPoint_;
}

const Vec3D RT::Ray::GetDirection() const {
    return direction_;
}

void RT::Ray::Reflect(const Vec3D &reflectNormal, const Vec3D &rayStart) {
    direction_ = direction_ - 2. * reflectNormal.dot(direction_) * reflectNormal;
    direction_.normalize();

    startPoint_ = rayStart;
    screenPoint_ = startPoint_ + Utils::DEFAULT_CAMERA_LENGTH * direction_;
}

void RT::Ray::SetStartPoint(Vec3D &startPoint) {
    startPoint_ = startPoint;
}

void RT::Ray::SetScreenPoint(Vec3D &screenPoint) {
    screenPoint_ = screenPoint;
}

void RT::Ray::SetDirection(Vec3D &direction) {
    direction_ = direction.normalized();
}

RT::HitPayload RT::Ray::RayIntersect(std::shared_ptr<RT::Object> pObject) {
    RT::ObjectType objType = pObject->GetType();
    if (objType == RT::ObjectType::TRIANGLE){
        RT::Triangle* pTriangle = static_cast<RT::Triangle*>(pObject.get());
        Vec3D triangleNormal = pTriangle->GetNormal();
        return RayTriangleIntersect(pTriangle->GetPointA(), pTriangle->GetPointB(), pTriangle->GetPointC(),
                                    pTriangle->GetEdgeAB(), pTriangle->GetEdgeAC(),
                                    triangleNormal, triangleNormal, triangleNormal);
    } else if (objType == RT::ObjectType::TRIANGLE_MESH){
        RT::TriangleMesh* pTriangleMesh = static_cast<RT::TriangleMesh*>(pObject.get());
        return RayTriangleMeshIntersect(pTriangleMesh);
    } else{
        throw std::invalid_argument("Base type object cannot be intersected");
    }
}

RT::HitPayload RT::Ray::RayTriangleMeshIntersect(RT::TriangleMesh *pTriangleMesh) {
    RT::HitPayload closestPayload;

    const auto& pVertices = pTriangleMesh->GetVertices();
    const auto& pEdges = pTriangleMesh->GetEdges();
    const auto& pTriangles = pTriangleMesh->GetTriangles();
    const auto& pVertexNormals = pTriangleMesh->GetVertexNormals();
    const auto& pNormals = pTriangleMesh->GetNormals();

    size_t i = 0;
    for (const auto& triangle : pTriangles){
#ifdef __SMOOTHING__
        RT::HitPayload payload = RayTriangleIntersect(pVertices[triangle[0]], pVertices[triangle[1]],pVertices[triangle[2]],
                                                   pEdges[i].first, pEdges[i].second,
                                                   pVertexNormals[triangle[0]], pVertexNormals[triangle[1]], pVertexNormals[triangle[2]]);
#else
        auto normal = pNormals[i];
        RT::HitPayload payload = RayTriangleIntersect(pVertices[triangle[0]], pVertices[triangle[1]],pVertices[triangle[2]],
                                                   pEdges[i].first, pEdges[i].second,
                                                   normal, normal, normal);
#endif
        if (payload.hitDist < closestPayload.hitDist){
            closestPayload = payload;
        }
        ++i;
    }
    return closestPayload;
}

RT::HitPayload RT::Ray::RayTriangleIntersect(const Vec3D &pointA, const Vec3D &pointB, const Vec3D &pointC,
                                          const Vec3D &edgeAB, const Vec3D &edgeAC,
                                          const Vec3D &hitNormal1, const Vec3D &hitNormal2, const Vec3D &hitNormal3) {
    RT::HitPayload payload;
    Vec3D pVec = cross(direction_, edgeAC);
    double det = edgeAB.dot(pVec);
    if (abs(det) < Utils::PARALLEL_PRECISION) return payload;

    double invDet = 1 / det;
    Vec3D tVec = startPoint_ - pointA;

    payload.u = tVec.dot(pVec) * invDet;
    if (payload.u < 0 || payload.u > 1) return payload;

    Vec3D qVec = cross(tVec, edgeAB);
    payload.v = direction_.dot(qVec) * invDet;
    if (payload.v < 0 || payload.u + payload.v > 1) return payload;

    double hitDist = edgeAC.dot(qVec) * invDet;
    if (hitDist < 0) return payload;
    payload.hitDist = hitDist;
    payload.hitNormal = (1 - payload.u - payload.v) * hitNormal1 + payload.u * hitNormal2 + payload.v * hitNormal3;

//    RT::HitPayload payload;
//    Vec3D N = edgeAB.cross(edgeAC);
//    double NdotRayDirection = N.dot(direction_);
//    if (abs(NdotRayDirection) < Utils::PARALLEL_PRECISION) return payload;
//
//    double d = -N.dot(pointA);
//    double hitDist = -(N.dot(startPoint_) + d) / NdotRayDirection;
//    if (hitDist < 0) return payload;
//
//    Vec3D C;
//    Vec3D hitPoint = startPoint_ + hitDist * direction_;
//    C = edgeAB.cross(hitPoint - pointA);
//    if (N.dot(C) < 0) return payload;
//
//    Vec3D edgeBC = pointC - pointB;
//    C = edgeBC.cross(hitPoint - pointB);
//    if (N.dot(C) < 0) return payload;
//
//    Vec3D edgeCA = pointA - pointC;
//    C = edgeCA.cross(hitPoint - pointC);
//    if (N.dot(C) < 0) return payload;
//
//    payload.hitDist = hitDist;
//    payload.hitNormal = hitNormal1;

    return payload;
}

const Vec3D RT::Ray::GetReflected(Vec3D& reflectNormal) const {
    Vec3D newDirection = direction_ - 2. * reflectNormal.dot(direction_) * reflectNormal;
    newDirection.normalize();
    return newDirection;
}
