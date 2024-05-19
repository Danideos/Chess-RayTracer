#include "Material.h"
#include "../Random/Random.h"

bool RT::Lambertian::Scatter(const RT::Ray &ray, const RT::HitPayload &hitPayload, RT::ScatterPayload &scatterPayload, int depth) const {
    scatterPayload.damping = albedo_;
    scatterPayload.pPDF = std::make_shared<RT::CosinePDF>(hitPayload.hitNormal);
    scatterPayload.skipPDF = false;
    return true;
}

double RT::Lambertian::ScatteringPDF(const RT::Ray& ray, const RT::HitPayload& hitPayload, const RT::Ray& scattered) const {
    auto cosineTheta = dot(hitPayload.hitNormal, scattered.GetDirection().normalized());
    return std::max(0., cosineTheta / M_PI);
}

RT::Lambertian::Lambertian(const Vec3D albedo) {
    albedo_ = albedo;
}

std::shared_ptr<RT::Material> RT::Lambertian::Clone() const {
    return std::make_unique<RT::Lambertian>(*this);
}

bool RT::Metal::Scatter(const RT::Ray& ray, const RT::HitPayload& hitPayload, RT::ScatterPayload &scatterPayload, int depth) const {
    Vec3D reflected = ray.GetReflected(hitPayload.hitNormal);
    reflected = reflected.normalized() + (fuzz_ * Rand::RandomUnitVector());

    scatterPayload.skipPDFRay = RT::Ray(hitPayload.hitPoint, hitPayload.hitPoint + reflected);
    scatterPayload.pPDF = nullptr;
    scatterPayload.skipPDF = true;
    scatterPayload.damping = albedo_;
    return true;

}

RT::Metal::Metal(const Vector<double, DIMS_3D> albedo, double fuzz) {
    albedo_ = albedo;
    fuzz_ = fuzz;
}

std::shared_ptr<RT::Material> RT::Metal::Clone() const {
    return std::make_unique<RT::Metal>(*this);
}

RT::Dielectric::Dielectric(double refractionIndex, Vec3D albedo, Vec3D transparency) {
    refractionIndex_ = refractionIndex;
    albedo_ = albedo;
    transparency_ = transparency;
}

bool RT::Dielectric::Scatter(const RT::Ray &ray, const RT::HitPayload &hitPayload,
                             RT::ScatterPayload &scatterPayload, int depth) const {
    scatterPayload.damping = (depth == Utils::BOUNCES) ? albedo_ : transparency_;
    scatterPayload.pPDF = nullptr;
    scatterPayload.skipPDF = true;

    double ri = hitPayload.frontFace ? (1.0 / refractionIndex_) : refractionIndex_;

    Vec3D unitDirection = ray.GetDirection().normalized();
    double cosTheta = fmin(dot(-unitDirection, hitPayload.hitNormal), 1.0);
    double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

    bool cannotRefract = ri * sinTheta > 1.0;

    Vec3D direction;
    if (cannotRefract || Reflectance(cosTheta, ri) > Rand::RandomDouble())
        direction = ray.GetReflected(hitPayload.hitNormal);
    else
        direction = ray.GetRefracted(hitPayload.hitNormal, ri);

    scatterPayload.skipPDFRay = RT::Ray(hitPayload.hitPoint + direction * 0.001, hitPayload.hitPoint + direction);
    return true;
}

double RT::Dielectric::Reflectance(double cosine, double refractionIndex) {
    auto r0 = (1 - refractionIndex) / (1 + refractionIndex);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

std::shared_ptr<RT::Material> RT::Dielectric::Clone() const {
    return std::make_unique<RT::Dielectric>(*this);
}
