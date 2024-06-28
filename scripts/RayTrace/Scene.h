/**
 * @file Scene.h
 * @brief Defines the Scene class used for ray tracing.
 */

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
    /**
     * @class Scene
     * @brief Represents a scene in the ray tracing environment.
     *
     * This class connects the entire ray tracing process. It is responsible for shooting rays,
     * calculating corresponding pixel colors, rendering the app window, and saving the final image.
     */
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
        void SetPixelColor(size_t x, size_t y, Vec3D &pixelColor);
        Vec3D ConvertToRGB(const Vec3D &color);
        /**
         * @brief Calculates the color at a hit point.
         *
         * @param ray The ray that hit the point.
         * @param depth The remaining depth for recursive ray tracing.
         * @return The color at the hit point.
         */
        Vec3D CalculateHitColor(RT::Ray &ray, int depth);
        /**
         * @brief Traces a ray to check for intersections.
         *
         * @param ray The ray to trace.
         * @param payload The payload to store hit information.
         * @return True if the ray intersects an object, false otherwise.
         */
        bool RayTrace(RT::Ray &ray, RT::HitPayload& payload);
        /**
         * @brief Calculates the bounding boxes for objects in the scene.
         */
        void CalculateBoundingBoxes();
        /**
         * @brief Creates a BMP file for the rendered image.
         *
         * @return An output file stream for the BMP file.
         */
        std::ofstream CreateBmpFile() const;
        void WriteColor(Vec3D color, std::ofstream& image);

        // Unfinished
        void ForTriangleRasterization(Vec3D A, Vec3D B, Vec3D C, Vec3D normal, std::vector<double> &depthBuffer);

    public:
        /// \brief Scene constructor, creates objects that belong to scene
        Scene();

        /// \brief Sets up screen height and width, initializes SDL texture and renderer
        void Initialize(size_t width, size_t height, SDL_Renderer *pRenderer);
        /// \brief Displays stored color channels as pixels using SDL
        void Display();
        /// \brief Renders the screen pixel colors using raytracing for each pixel
        bool Render();
    };
}

#endif