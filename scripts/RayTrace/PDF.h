#ifndef MAIN_CPP_PDF_H
#define MAIN_CPP_PDF_H

#include "../Utilities/Utils.h"
#include <cstdlib>
#include <math.h>
#include <vector>

namespace RT{

    class PDF{
    public:
        virtual double Value(Vec3D direction) const = 0;
        virtual Vec3D Generate() const = 0;
    };

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

