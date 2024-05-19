#ifndef MAIN_CPP_MATERIAL_H
#define MAIN_CPP_MATERIAL_H

#include "Ray.h"
#include "PDF.h"
#include "../Utilities/Utils.h"

namespace RT{
    struct ScatterPayload{
        Vec3D damping;
        bool skipPDF;
        RT::Ray skipPDFRay;
        std::shared_ptr<PDF> pPDF;
    };

    class Material {
    public:
        virtual bool Scatter(const RT::Ray& ray, const RT::HitPayload& hitPayload, RT::ScatterPayload& scatterPayload, int depth) const { return false; }
        virtual double ScatteringPDF(const RT::Ray& ray, const RT::HitPayload& hitPayload, const RT::Ray& scattered) const { return false; };
        virtual std::shared_ptr<RT::Material> Clone() const = 0;

        virtual void SetAlbedo(Vec3D albedo) const {};
        virtual void SetFuzz(double fuzz) const {};
        virtual void SetRefractionIndex(double refraction) const {};
    };

    class Lambertian : public Material{
    public:
        Lambertian(const Vec3D albedo);

        bool Scatter(const RT::Ray& ray, const RT::HitPayload& hitPayload, RT::ScatterPayload& scatterPayload, int depth) const override;
        double ScatteringPDF(const RT::Ray& ray, const RT::HitPayload& hitPayload, const RT::Ray& scattered) const override;

        void SetAlbedo(Vec3D albedo) { albedo_ = albedo; }

        std::shared_ptr<RT::Material> Clone() const override;

    private:
        Vec3D albedo_;
    };

    class Metal : public Material {
    public:
        Metal(const Vec3D albedo, double fuzz);

        bool Scatter(const RT::Ray& ray, const RT::HitPayload& hitPayload, RT::ScatterPayload& scatterPayload, int depth) const override;

        void SetAlbedo(Vec3D albedo) { albedo_ = albedo; }
        void SetFuzz(double fuzz) { fuzz_ = fuzz; }

        std::shared_ptr<RT::Material> Clone() const override;

    private:
        Vec3D albedo_;
        double fuzz_;
    };

    class Dielectric : public Material {
    public:
        Dielectric(double refractionIndex = Utils::BASE_REFRACTION_INDEX, Vec3D albedo = Utils::BASE_ALBEDO,
                   Vec3D transparency = Utils::BASE_TRANSPARENCY);

        bool Scatter(const RT::Ray& ray, const RT::HitPayload& hitPayload, RT::ScatterPayload& scatterPayload, int depth) const override;
        void SetRefractionIndex(double refraction) { refractionIndex_ = refraction; }
        Vec3D GetAlbedo() const { return albedo_; }

        std::shared_ptr<RT::Material> Clone() const override;

    private:
        Vec3D albedo_;
        double refractionIndex_;
        Vec3D transparency_;

        static double Reflectance(double cosine, double refraction_index);
    };
}

#endif
