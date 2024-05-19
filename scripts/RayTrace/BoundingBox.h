#ifndef MAIN_CPP_BOUNDINGBOX_H
#define MAIN_CPP_BOUNDINGBOX_H

#include "Objects.h"
#include "Material.h"
#include <vector>
#include <memory>

namespace RT{
    class BoundingBox{
    public:
        BoundingBox(std::shared_ptr<RT::Object> pObject);

        std::vector<std::shared_ptr<RT::Object>> GetBoundingBox() const;
        std::vector<std::shared_ptr<RT::Object>> GetObjectList() const;

    private:
        std::vector<std::shared_ptr<RT::Object>> pInsideObjectList_;
        std::vector<std::shared_ptr<RT::Object>> pBoxObjectList_;
    };
}

#endif
