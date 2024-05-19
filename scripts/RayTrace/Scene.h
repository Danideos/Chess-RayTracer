#ifndef MAIN_CPP_SCENE_H
#define MAIN_CPP_SCENE_H

#include <SDL.h>
#include <limits.h>
#include <vector>
#include <memory>
#include <algorithm>
#include <thread>

#include "../LinearAlgebra/Vector.h"
#include "../ObjectLoader/ObjLoader.h"
#include "../Utilities/Utils.h"
#include "../Log.h"
#include "Camera.h"
#include "Ray.h"
#include "Objects.h"
#include "Material.h"
#include "BoundingBox.h"


namespace RT{
    class Scene{
    private:
        SDL_Texture *pTexture_;
        SDL_Renderer *pRenderer_;

        RT::Camera camera_;
        std::vector<std::shared_ptr<RT::Object>> pObjectList_;
        RT::DistantLightSource light_;

        std::vector<std::vector<double>> rColor_;
        std::vector<std::vector<double>> gColor_;
        std::vector<std::vector<double>> bColor_;

        size_t sceneWidth_;
        size_t sceneHeight_;

        std::vector<RT::BoundingBox> boundingBoxes_;

        bool rasterization_;
        std::vector<std::shared_ptr<RT::Object>> rasterScreen_;

    private:
        uint32_t ConvertToInt32(double red, double green, double blue);
        Vec3D CalculateHitColor(RT::Ray &ray, int depth);
        void SetPixelColor(size_t x, size_t y, Vec3D &pixelColor);
        Vec3D ConvertToRGB(const Vec3D &color);

        bool RayTrace(RT::Ray &ray, RT::HitPayload& payload);

        void CalculateBoundingBoxes();

        std::ofstream CreateBmpFile() const;
        void WriteColor(Vec3D color, std::ofstream& image);

        // Unfinished
        void ForTriangleRasterization(Vec3D A, Vec3D B, Vec3D C, Vec3D normal, std::vector<double> &depthBuffer);

    public:
        Scene();

        void Initialize(size_t width, size_t height, SDL_Renderer *pRenderer);
        void Display();
        bool Render();
    };
}

#endif