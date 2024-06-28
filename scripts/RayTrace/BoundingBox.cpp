#include "BoundingBox.h"
#include <iostream>

RT::BoundingBox::BoundingBox(std::shared_ptr<RT::Object> pObject) {
    pInsideObjectList_.emplace_back(pObject);
    if (pObject->GetType() == RT::ObjectType::TRIANGLE){
        pBoxObjectList_.emplace_back(pObject);
    } else {
        auto [minPoint, maxPoint] = pObject->GetBoundingPoints();

        Vec3D v0{minPoint[0], minPoint[1], minPoint[2]};
        Vec3D v1{maxPoint[0], minPoint[1], minPoint[2]};
        Vec3D v2{maxPoint[0], maxPoint[1], minPoint[2]};
        Vec3D v3{minPoint[0], maxPoint[1], minPoint[2]};
        Vec3D v4{minPoint[0], minPoint[1], maxPoint[2]};
        Vec3D v5{maxPoint[0], minPoint[1], maxPoint[2]};
        Vec3D v6{maxPoint[0], maxPoint[1], maxPoint[2]};
        Vec3D v7{minPoint[0], maxPoint[1], maxPoint[2]};
        pBoxObjectList_.emplace_back(std::make_shared<RT::Triangle>(v1, v6, v5));
        pBoxObjectList_.emplace_back(std::make_shared<RT::Triangle>(v1, v2, v6));
        pBoxObjectList_.emplace_back(std::make_shared<RT::Triangle>(v0, v2, v1));
        pBoxObjectList_.emplace_back(std::make_shared<RT::Triangle>(v0, v3, v2));
        pBoxObjectList_.emplace_back(std::make_shared<RT::Triangle>(v4, v5, v6));
        pBoxObjectList_.emplace_back(std::make_shared<RT::Triangle>(v4, v6, v7));
        pBoxObjectList_.emplace_back(std::make_shared<RT::Triangle>(v0, v7, v3));
        pBoxObjectList_.emplace_back(std::make_shared<RT::Triangle>(v0, v3, v7));
        pBoxObjectList_.emplace_back(std::make_shared<RT::Triangle>(v3, v6, v2));
        pBoxObjectList_.emplace_back(std::make_shared<RT::Triangle>(v3, v7, v6));
        pBoxObjectList_.emplace_back(std::make_shared<RT::Triangle>(v0, v5, v4));
        pBoxObjectList_.emplace_back(std::make_shared<RT::Triangle>(v0, v1, v5));
        auto base_material = std::make_shared<RT::Lambertian>(RT::Lambertian(Utils::BASE_ALBEDO));
        for (auto pBox: pBoxObjectList_) {
            pBox->SetMaterial(base_material);
        }
    }
}

std::vector<std::shared_ptr<RT::Object>> RT::BoundingBox::GetBoundingBox() const {
    return pBoxObjectList_;
}

std::vector<std::shared_ptr<RT::Object>> RT::BoundingBox::GetObjectList() const {
    return pInsideObjectList_;
}
