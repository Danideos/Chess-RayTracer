#ifndef MAIN_CPP_PDF_H
#define MAIN_CPP_PDF_H

#include "../Utilities/Utils.h"

namespace RT{

    class PDF{
    public:
        virtual double value(const Vec3D& direction) const = 0;
        virtual Vec3D generate() const = 0;
    };

    class CosidePDF : public PDF{
    public:
        CosidePDF
    };
}

#endif

