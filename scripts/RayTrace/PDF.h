/**
 * @file PDF.h
 * @brief Defines various probability density functions (PDFs) used for ray scattering in ray tracing.
 */
#ifndef MAIN_CPP_PDF_H
#define MAIN_CPP_PDF_H

#include "../Utilities/Utils.h"
#include <cstdlib>
#include <math.h>
#include <vector>

namespace RT{
    /**
     * @class PDF
     * @brief Abstract base class for all probability density functions used in ray tracing.
     */
    class PDF{
    public:
        /**
         * \brief Calculates the PDF value
         * @param direction Direction of the scattered ray
         * @return Return PDF value, which represents how much does scattered ray contribute to the hit place color
         */
        virtual double Value(Vec3D direction) const = 0;
        /** \brief Calculates random scatter direction
         *
         * @return Random scatter direction according to some distribution
         */
        virtual Vec3D Generate() const = 0;
    };
    /**
     * @class CosinePDF
     * @brief Represents a cosine-weighted probability density function.
     */
    class CosinePDF : public PDF{
    public:
        CosinePDF(Vec3D hitNormal);

        double Value(Vec3D direction) const override;
        Vec3D Generate() const override;

    private:
        Vec3D RandomCosineDirection() const;
        Vec3D Local(Vec3D a) const;

        std::vector<Vec3D> axis_;
    };
}

#endif

