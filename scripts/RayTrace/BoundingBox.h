/**
 * @file BoundingBox.h
 * @brief Defines the BoundingBox class used for ray tracing.
 */
#ifndef MAIN_CPP_BOUNDINGBOX_H
#define MAIN_CPP_BOUNDINGBOX_H

#include "Objects.h"
#include "Material.h"
#include <vector>
#include <memory>

namespace RT{
    /**
     * @class BoundingBox
     * @brief Represents a bounding box in the ray tracing environment.
     *
     * The BoundingBox class is used to define a rectangular region in 3D space
     * which can be used for efficient ray-object intersection tests.
     */
    class BoundingBox{
    public:
        /**
          * @brief Constructs a new BoundingBox object with specified minimum and maximum coordinates.
          *
          * @param min The minimum coordinates of the bounding box {xmin, ymin, zmin}.
          * @param max The maximum coordinates of the bounding box {xmax, ymax, zmax}.
          */
        BoundingBox(std::shared_ptr<RT::Object> pObject);

        std::vector<std::shared_ptr<RT::Object>> GetBoundingBox() const;
        std::vector<std::shared_ptr<RT::Object>> GetObjectList() const;

    private:
        std::vector<std::shared_ptr<RT::Object>> pInsideObjectList_;
        std::vector<std::shared_ptr<RT::Object>> pBoxObjectList_;
    };
}

#endif
