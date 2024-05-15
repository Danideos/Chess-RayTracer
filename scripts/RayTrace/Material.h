#ifndef MAIN_CPP_MATERIAL_H
#define MAIN_CPP_MATERIAL_H

#include "Ray.h"
#include "PDF.h"
#include "../Utilities/Utils.h"

namespace RT{
    struct ScatterPayload{
        Vec3D damping;
        bool skipPdf;
        RT::Ray skipPdfRay;
        std::shared_ptr<PDF> pPdf;
    };

    class Material {
    public:
        virtual bool scatter(const RT::Ray& ray, RT::HitPayload hitPayload, RT::ScatterPayload scatterPayload) const {
            return false;
        }

    private:

    };
}


#endif
