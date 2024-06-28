/**
 * @file Material.h
 * @brief Defines various material classes used for ray tracing.
 */
#ifndef MAIN_CPP_MATERIAL_H
#define MAIN_CPP_MATERIAL_H

#include "Ray.h"
#include "PDF.h"
#include "../Utilities/Utils.h"

namespace RT{
    /**
     * @struct ScatterPayload
     * @brief Stores data related to ray scattering.
     */
    struct ScatterPayload{
        Vec3D damping;/**< The damping factor applied to the ray. */
        bool skipPDF; /**< Flag indicating whether to skip the PDF calculation. */
        RT::Ray skipPDFRay; /**< The ray to use if skipping the PDF calculation. */
        std::shared_ptr<PDF> pPDF; /**< Pointer to the probability density function for scattering. */
    };
    /**
     * @class Material
     * @brief Abstract base class for all materials in the ray tracing environment.
     *
     * The Material class defines the interface for materials, including how rays interact
     * with them through scattering and reflection.
     */
    class Material {
    public:
        /** \brief Defines ray behaviour after collision with particular material
         *
         * @param ray Ray to scatter
         * @param hitPayload Hit payload for info about the hit
         * @param scatterPayload Scatter payload for info about the scattering
         * @param depth Remaining bounces of the ray
         * @return Returns whether the ray should scatter or reflect and sets up scatter payload params such as PDF, scattered ray...
         */
        virtual bool Scatter(const RT::Ray& ray, const RT::HitPayload& hitPayload, RT::ScatterPayload& scatterPayload, int depth) const { return false; }
        /** \brief Calculates PDF of the scattered ray
         *
         * @param ray Original ray
         * @param hitPayload Hit payload for info about the hit
         * @param scattered Scattered ray
         * @return Returns how much light does the ray collect according to material PDF and the angle of scattered ray
         */
        virtual double ScatteringPDF(const RT::Ray& ray, const RT::HitPayload& hitPayload, const RT::Ray& scattered) const { return false; };
        virtual std::shared_ptr<RT::Material> Clone() const = 0;

        virtual void SetAlbedo(Vec3D albedo) const {};
        virtual void SetFuzz(double fuzz) const {};
        virtual void SetRefractionIndex(double refraction) const {};
    };
    /**
     * @class Lambertian
     * @brief Represents a Lambertian (diffuse, wood like) material.
     */
    class Lambertian : public Material{
    public:
        Lambertian(const Vec3D albedo);

        bool Scatter(const RT::Ray& ray, const RT::HitPayload& hitPayload, RT::ScatterPayload& scatterPayload, int depth) const override;
        double ScatteringPDF(const RT::Ray& ray, const RT::HitPayload& hitPayload, const RT::Ray& scattered) const override;

        void SetAlbedo(Vec3D albedo) { albedo_ = albedo; }

        std::shared_ptr<RT::Material> Clone() const override;

    private:
        Vec3D albedo_; /**< The albedo (reflectivity) of the material. */
    };

    /**
     * @class Metal
     * @brief Represents a metallic material.
     */
    class Metal : public Material {
    public:

        Metal(const Vec3D albedo, double fuzz);

        bool Scatter(const RT::Ray& ray, const RT::HitPayload& hitPayload, RT::ScatterPayload& scatterPayload, int depth) const override;

        void SetAlbedo(Vec3D albedo) { albedo_ = albedo; }
        void SetFuzz(double fuzz) { fuzz_ = fuzz; }

        std::shared_ptr<RT::Material> Clone() const override;

    private:
        Vec3D albedo_; /**< The albedo (reflectivity) of the material. */
        double fuzz_; /**< The fuzziness factor affecting the reflection. */
    };

    /**
     * @class Dielectric
     * @brief Represents a dielectric (transparent) material.
     */
    class Dielectric : public Material {
    public:
        Dielectric(double refractionIndex = Utils::BASE_REFRACTION_INDEX, Vec3D albedo = Utils::BASE_ALBEDO,
                   Vec3D transparency = Utils::BASE_TRANSPARENCY);

        bool Scatter(const RT::Ray& ray, const RT::HitPayload& hitPayload, RT::ScatterPayload& scatterPayload, int depth) const override;
        void SetRefractionIndex(double refraction) { refractionIndex_ = refraction; }
        Vec3D GetAlbedo() const { return albedo_; }

        std::shared_ptr<RT::Material> Clone() const override;

    private:
        Vec3D albedo_; /**< The albedo (reflectivity) of the material. */
        double refractionIndex_;  /**< The refraction index of the material / how much the light refracts on contact. */
        Vec3D transparency_; /**< The transparency of the material. */
        /**
         * @brief Calculates the reflectance using Schlick's approximation.
         *
         * @param cosine The cosine of the angle of incidence.
         * @param refraction_index The refraction index.
         * @return The reflectance value.
         */
        static double Reflectance(double cosine, double refraction_index);
    };
}

#endif
