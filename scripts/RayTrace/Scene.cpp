#include "Scene.h"
#include "Chessboard.h"

RT::Scene::Scene() {
    // Load object from .obj file
    auto king = ObjLoader::loadTriangleMeshObj("king.obj");
    king->SetColor(Utils::WHITE_PIECE_COLOR);
    king->Fit1x1(0.8, 0.8);
    king->SetCenter(Vec3D{2.5, 0., 5.5});
    pObjectList_.push_back(king);

    auto queen = ObjLoader::loadTriangleMeshObj("queen.obj");
    queen->SetColor(Utils::BLACK_PIECE_COLOR);
    queen->Fit1x1(0.8, 0.8);
    queen->SetCenter(Vec3D{5.5, 0., 2.5});
    pObjectList_.push_back(queen);

    auto pawn1 = ObjLoader::loadTriangleMeshObj("pawn.obj");
    pObjectList_.push_back(pawn1);
    pawn1->SetColor(Utils::BLACK_PIECE_COLOR);
    pawn1->Fit1x1(0.6, 0.6);
    pawn1->SetCenter(Vec3D{0.5, 0., 2.5});

    auto pawn2 = ObjLoader::loadTriangleMeshObj("pawn.obj");
    pawn2->SetColor(Utils::WHITE_PIECE_COLOR);
    pawn2->Fit1x1(0.6, 0.6);
    pawn2->SetCenter(Vec3D{4.5, 0., 4.5});
    pObjectList_.push_back(pawn2);

    // Create base plane
    Chessboard chessboard(Vec3D{0., 0., 0.5});
    auto pObjListChessboard = chessboard.GetObjectPointers();
    for (auto pObj : pObjListChessboard){
        pObjectList_.push_back(pObj);
    }

//    auto plane = std::make_shared<RT::Triangle>(Vec3D{0., 0., 0.}, Vec3D{8., 0., 0.}, Vec3D{8., 8., 0.});
//    pObjectList_.push_back(plane);
//    auto plane2 = std::make_shared<RT::Triangle>(Vec3D{0., 8., 0.}, Vec3D{0., 0., 0.}, Vec3D{8., 8., 0.});
//    pObjectList_.push_back(plane2);
//    auto plane3 = std::make_shared<RT::Triangle>(Vec3D{0., 0., 0.}, Vec3D{0., 8., 0.}, Vec3D{0., 0., 8.});
//    pObjectList_.push_back(plane3);
//    auto plane4 = std::make_shared<RT::Triangle>(Vec3D{8., 8., 0.}, Vec3D{8., 0., 8.}, Vec3D{8., 0., 0.});
//    pObjectList_.push_back(plane4);
}

void RT::Scene::Initialize(size_t width, size_t height, SDL_Renderer *pRend){
    sceneWidth_ = width;
    sceneHeight_ = height;
    pRenderer_ = pRend;

    rColor_.resize(sceneWidth_, std::vector<double>(sceneHeight_, Utils::RGB_MAX));
    gColor_.resize(sceneWidth_, std::vector<double>(sceneHeight_, Utils::RGB_MAX));
    bColor_.resize(sceneWidth_, std::vector<double>(sceneHeight_, Utils::RGB_MAX));

    uint32_t rMask, gMask, bMask, aMask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rMask = 0xff000000;
    gMask = 0x00ff0000;
    bMask = 0x0000ff00;
    aMask = 0x000000ff;
#else
    rMask = 0x000000ff;
    gMask = 0x0000ff00;
    bMask = 0x00ff0000;
    aMask = 0xff000000;
#endif

    if (pTexture_ != nullptr){
        SDL_DestroyTexture(pTexture_);
    }
    SDL_Surface *temp = SDL_CreateRGBSurface(0, sceneWidth_, sceneHeight_, 32, rMask, gMask, bMask, aMask);
    pTexture_ = SDL_CreateTextureFromSurface(pRenderer_, temp);
    SDL_FreeSurface(temp);

}

uint32_t RT::Scene::ConvertToInt32(double red, double green, double blue) {
    unsigned char r = static_cast<unsigned char>(red);
    unsigned char g = static_cast<unsigned char>(green);
    unsigned char b = static_cast<unsigned char>(blue);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    Uint32 color = (r << 24) + (g << 16) + (b << 8) + 255;
#else
    Uint32 color = (255 << 24) + (r << 16) + (g << 8) + b;
#endif

    return color;
}

bool RT::Scene::Render() {
    double xFact = 1. / (sceneWidth_);
    double yFact = 1. / (sceneHeight_);
#ifdef __MT__
    std::vector<std::thread> threads;

    for (size_t y = 0; y < sceneHeight_; ++y) {
        // Launch a thread for each row
        threads.emplace_back([this, xFact, yFact, y]() {
            for (size_t x = 0; x < sceneWidth_; ++x) {
                // Normalize the pixels according to screen size and center
                double xNorm = x * xFact - 0.5;
                double yNorm = y * yFact - 0.5;
                // Create and cast ray
                RT::Ray ray = camera_.GetRay(xNorm, yNorm);
                Vec3D pixelColor = PerPixel(ray);
                // Convert and set pixel on screen
                Vec3D pixelColorRGB = ConvertToRGB(pixelColor);
                SetPixelColor(x, y, pixelColorRGB);
            }
        });
        SDL_RenderClear(pRenderer_);
        Display();
        SDL_RenderPresent(pRenderer_);
    }
    for (auto& thread : threads) {
        thread.join();
    }
#else
//    for (size_t y = sceneHeight_ - 1; y != SIZE_MAX; --y){
    for (size_t y = 0; y < sceneHeight_; ++y){
        for (size_t x = 0; x < sceneWidth_; ++x){
            double xNorm = x * xFact - 0.5;
            double yNorm = y * yFact - 0.5;
            RT::Ray ray = camera_.GetRay(xNorm, yNorm);
            std::cout << "x, y: " << x << ", " << y << std::endl;
            Vec3D pixelColor = PerPixel(ray);

            Vec3D pixelRGBColor = ConvertToRGB(pixelColor);
            SetPixelColor(x, y, pixelRGBColor);
        }
        SDL_RenderClear(pRenderer_);
        Display();
        SDL_RenderPresent(pRenderer_);
    }
#endif
    SDL_RenderClear(pRenderer_);
    Display();
    SDL_RenderPresent(pRenderer_);
    return true;
}

void RT::Scene::Display() {
    uint32_t *pixelColors = new uint32_t[sceneWidth_ * sceneHeight_];
    for (size_t x = 0; x < sceneWidth_; ++x){
        for (size_t y = 0; y < sceneHeight_; ++y){
            pixelColors[((sceneHeight_ - y - 1) * sceneWidth_) + x] = ConvertToInt32(rColor_[x][y], gColor_[x][y], bColor_[x][y]);
        }
    }

    SDL_UpdateTexture(pTexture_, nullptr, pixelColors, sizeof(uint32_t) * sceneWidth_);
    delete [] pixelColors;

    SDL_RenderCopy(pRenderer_, pTexture_, nullptr, nullptr);
}

Vec3D RT::Scene::PerPixel(RT::Ray &ray) {
    std::shared_ptr<RT::Object> pObject = nullptr;
    Vec3D pixelColor = Utils::EMPTY_COLOR;
    for (int i = 0; i < Utils::BOUNCES; ++i){
        double u, v, hitDist = DBL_MAX;
        Vec3D hitNormal;
        if (RayTrace(u, v, hitDist, hitNormal, pObject, ray)){
            double u2, v2, hitDist2 = DBL_MAX;
            Vec3D hitNormal2;
            Vec3D hitColor;
            std::shared_ptr<RT::Object> pObject2 = nullptr;

            Vec3D rayStart = ray.GetStartPoint();
            Vec3D rayDir = ray.GetDirection();
            Vec3D newRayStart = rayStart + hitDist * rayDir + hitNormal * 0.1;

            RT::Ray shadow_ray(newRayStart, newRayStart - light_.GetDirection());

            if (RayTrace(u2, v2, hitDist2, hitNormal2, pObject2, shadow_ray)){
                hitColor = CalculateHitColor(pObject, hitDist, hitNormal, u, v, rayDir) * 0.5;
            } else{
                hitColor = CalculateHitColor(pObject, hitDist, hitNormal, u, v, rayDir);
            }
            pixelColor = pixelColor + hitColor;

            rayStart = rayStart + hitDist * rayDir + hitNormal * 0.001;
            ray.Reflect(hitNormal, rayStart);

        } else{
            pixelColor = pixelColor + Vec3D{0.6, 0.7, 0.9};
        }
        pixelColor = pixelColor * 9;

    }
    pixelColor = pixelColor / 819;

    return pixelColor;
}

Vec3D RT::Scene::CalculateHitColor(std::shared_ptr<RT::Object> pObject, double hitDist, Vec3D &hitNormal,
                                    double &u, double &v, Vec3D &rayDir) {
    Vec3D color = pObject->GetColor() * light_.GetIntensity() * pObject->GetAlbedo() / M_PI *
            std::max(0., dot(hitNormal, - light_.GetDirection()));
    for (int i = 0; i < 3; ++i){
        color[i] = std::min(1., color[i]);
    }
    return color;
}

Vec3D RT::Scene::ConvertToRGB(const Vec3D &color) {
    Vec3D colorRGB = Vec3D{color[0] * Utils::RGB_MAX, color[1] * Utils::RGB_MAX, color[2] * Utils::RGB_MAX};
    return colorRGB;
}

void RT::Scene::SetPixelColor(size_t x, size_t y, Vec3D &pixelColor) {
    rColor_[x][y] = pixelColor[0];
    gColor_[x][y] = pixelColor[1];
    bColor_[x][y] = pixelColor[2];
}

bool RT::Scene::RayTrace(double &closestU, double &closestV, double &closestHitDist, Vec3D &closestHitNormal,
                         std::shared_ptr<RT::Object> &pClosestObject, RT::Ray &ray) {
    for (const auto pObject : pObjectList_){
        RT::HitPayload payload = ray.RayIntersect(pObject);
        if (payload.hitDist < closestHitDist && payload.hitDist < 1e9){
            closestHitDist = payload.hitDist;
            closestU = payload.u;
            closestV = payload.v;
            pClosestObject = pObject;
            closestHitNormal = payload.hitNormal;
        }
    }
    return closestHitDist != DBL_MAX;
}


