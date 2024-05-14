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

namespace RT{
    class Scene{
    private:
        RT::Camera camera_;
        std::vector<std::shared_ptr<RT::Object>> pObjectList_;
        RT::DistantLightSource light_;

        std::vector<std::vector<double>> rColor_;
        std::vector<std::vector<double>> gColor_;
        std::vector<std::vector<double>> bColor_;

        size_t sceneWidth_;
        size_t sceneHeight_;

        SDL_Texture *pTexture_;
        SDL_Renderer *pRenderer_;
    private:
        uint32_t ConvertToInt32(double red, double green, double blue);
        Vec3D CalculateHitColor(std::shared_ptr<RT::Object> pObject, double hitDist, Vec3D &hitNormal,
                                double &u, double &v, Vec3D &rayDir);
        void SetPixelColor(size_t x, size_t y, Vec3D &pixelColor);
        Vec3D ConvertToRGB(const Vec3D &color);

        Vec3D CastRay(RT::Ray &ray);
        bool RayTrace(double &u, double& v, double &hitDist, Vec3D &hitNormal, std::shared_ptr<RT::Object> &pObject,
                      RT::Ray &ray);

    public:
        Scene();

        void Initialize(size_t width, size_t height, SDL_Renderer *pRenderer);
        void Display();
        bool Render();
    };
}

#endif