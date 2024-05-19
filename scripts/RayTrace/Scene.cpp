#include "Scene.h"
#include "Chessboard.h"

RT::Scene::Scene() {
    auto blue_material_metal = std::make_shared<RT::Metal>(Vec3D{0.6, 0.7, 0.9}, 0.0);
    auto pink_material_metal = std::make_shared<RT::Metal>(Vec3D{1., 0.35, 1.}, 0.01);

    // Create base plane
    Chessboard chessboard(Vec3D{0., 0., 0.});
    auto pObjListChessboard = chessboard.GetObjectPointers();
    for (auto pObj : pObjListChessboard){
        pObjectList_.push_back(pObj);
    }

    // Create metal mirrors for prettier scene
    auto plane = std::make_shared<RT::Triangle>(Vec3D{-1., -1., -1.}, Vec3D{9., -1., -1.}, Vec3D{9., 9., -1.});
    plane->SetMaterial(blue_material_metal);
    pObjectList_.push_back(plane);
    auto plane2 = std::make_shared<RT::Triangle>(Vec3D{-1., 9., -1.}, Vec3D{-1., -1., -1.}, Vec3D{9., 9., -1.});
    plane2->SetMaterial(blue_material_metal);
    pObjectList_.push_back(plane2);
    auto plane3 = std::make_shared<RT::Triangle>(Vec3D{-1., -1., -2.}, Vec3D{-1., -1., 9.}, Vec3D{-1., 9., 9.});
    plane3->SetMaterial(pink_material_metal);
    pObjectList_.push_back(plane3);
    auto plane4 = std::make_shared<RT::Triangle>(Vec3D{-1., 9., -2.}, Vec3D{-1., -1., -2.}, Vec3D{-1., 9., 9.});
    plane4->SetMaterial(pink_material_metal);
    pObjectList_.push_back(plane4);

    CalculateBoundingBoxes();

    rasterization_ = false;
//    Vec3D camCent = camera_.GetCenter();
//    Vec3D screenU = camera_.GetScreenU();
//    Vec3D screenV = camera_.GetScreenV();
//    auto screen1 = std::make_shared<RT::Triangle>(
//            camCent + 100. * (-screenU - screenV),
//            camCent + 100. * (-screenU + screenV),
//            camCent + 100. * (screenU + screenV)
//            );
//    rasterScreen_.push_back(screen1);
//    auto screen2 = std::make_shared<RT::Triangle>(
//            camCent + 100. * (screenU - screenV),
//            camCent + 100. * (screenU + screenV),
//            camCent + 100. * (-screenU - screenV)
//            );
//    rasterScreen_.push_back(screen2);
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
    if (rasterization_) {
        std::vector<double> depthBuffer;
        for (int i = 0; i < sceneWidth_ * sceneHeight_; ++i) { depthBuffer.emplace_back(DBL_MAX); }
        int i = 0;
        for (auto pObject : pObjectList_){
            if (pObject->GetType() == RT::ObjectType::TRIANGLE_MESH){
                RT::TriangleMesh* pTriangleMesh = static_cast<RT::TriangleMesh*>(pObject.get());
                auto triangles = pTriangleMesh->GetTriangles();
                auto normals = pTriangleMesh->GetNormals();
                auto vertices = pTriangleMesh->GetVertices();
                for (auto triangle : triangles){
                    ForTriangleRasterization(vertices[triangle[0]], vertices[triangle[1]], vertices[triangle[2]], normals[i], depthBuffer);
                }
                ++i;
            }
        }
    } else{
        double xFact = 1. / (sceneWidth_);
        double yFact = 1. / (sceneHeight_);
#ifdef __MT__
    std::vector<std::thread> threads;
    threads.reserve(sceneHeight_);
    for (size_t y = 0; y < sceneHeight_; ++y) {
        // Launch a thread for each row
        threads.emplace_back([this, xFact, yFact, y]() {
            for (size_t x = 0; x < sceneWidth_; ++x) {
                // Create and cast ray
                Vec3D pixelColor = Utils::EMPTY_COLOR;
                for (double i = 0; i < Utils::SQRT_SAMPLES; ++i){
                    for (double j = 0; j < Utils::SQRT_SAMPLES; ++j){
                        // Normalize the pixels according to screen size and center
                        double xNorm = (x + i / Utils::SQRT_SAMPLES) * xFact - 0.5;
                        double yNorm = (y + j / Utils::SQRT_SAMPLES) * yFact - 0.5;
                        RT::Ray ray = camera_.GetRay(xNorm, yNorm);
                        pixelColor = pixelColor + CalculateHitColor(ray, Utils::BOUNCES);
                    }
                }
                pixelColor = pixelColor / (Utils::SQRT_SAMPLES * Utils::SQRT_SAMPLES);
                // Convert and set pixel on screen
                Vec3D pixelColorRGB = ConvertToRGB(pixelColor);
                SetPixelColor(x, y, pixelColorRGB);
            }
        });
    }
    for (auto& thread : threads) {
        thread.join();
    }
#else
    for (size_t y = 0; y < sceneHeight_; ++y){
        for (size_t x = 0; x < sceneWidth_; ++x){
            Vec3D pixelColor = Utils::EMPTY_COLOR;
            std::cout << "x, y: " << x << ", " << y << std::endl;
            for (double i = 0; i < Utils::SQRT_SAMPLES; ++i){
                for (double j = 0; j < Utils::SQRT_SAMPLES; ++j){
                    // Normalize the pixels according to screen size and center
                    double xNorm = (x + i / Utils::SQRT_SAMPLES) * xFact - 0.5;
                    double yNorm = (y + j / Utils::SQRT_SAMPLES) * yFact - 0.5;
                    RT::Ray ray = camera_.GetRay(xNorm, yNorm);
                    pixelColor = pixelColor + CalculateHitColor(ray, Utils::BOUNCES);
                }
            }
            pixelColor = pixelColor / (Utils::SQRT_SAMPLES * Utils::SQRT_SAMPLES);
            Vec3D pixelRGBColor = ConvertToRGB(pixelColor);
            SetPixelColor(x, y, pixelRGBColor);
        }
    }
#endif
    }
    const int paddingAmount = ((4 - (sceneWidth_ * 3) % 4) % 4);
    unsigned char bmpPad[3] = { 0, 0, 0 };
    std::ofstream image = CreateBmpFile();
    for (int i = 0; i < sceneHeight_; ++i) {
        for (int j = 0; j < sceneWidth_; ++j) {
            Vec3D color{bColor_[j][i], gColor_[j][i], rColor_[j][i]};
            WriteColor(color, image);
        }
        image.write(reinterpret_cast<char*>(bmpPad), paddingAmount);
    }
    image.close();

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

Vec3D RT::Scene::CalculateHitColor(RT::Ray &ray, int depth) {
    if (depth <= 0) return Utils::EMPTY_COLOR;

    RT::HitPayload hitPayload;
    if (!RayTrace(ray, hitPayload)){
        return Utils::BACKGROUND_COLOR;
    }

    RT::ScatterPayload scatterPayload;
    if (!hitPayload.pObject->GetMaterial()->Scatter(ray, hitPayload, scatterPayload, depth)){
        return Utils::EMPTY_COLOR;
    }

    if (scatterPayload.skipPDF){
        return CalculateHitColor(scatterPayload.skipPDFRay, depth - 1) * scatterPayload.damping;
    }
    Vec3D scatteredRayDir = scatterPayload.pPDF->Generate();
    RT::Ray scatteredRay = RT::Ray(hitPayload.hitPoint + scatteredRayDir * 0.001, hitPayload.hitPoint + scatteredRayDir);
    auto valPDF = scatterPayload.pPDF->Value(scatteredRay.GetDirection());

    HitPayload lightHitPayload;
    RT::Ray lightRay = RT::Ray(hitPayload.hitPoint - light_.GetDirection() * 0.05 * scatteredRayDir,
                               hitPayload.hitPoint - light_.GetDirection() + 0.05 * scatteredRayDir);
    if (RayTrace(lightRay, lightHitPayload)) valPDF *= 2.;

    double scatteringPDF = hitPayload.pObject->GetMaterial()->ScatteringPDF(ray, hitPayload, scatteredRay);
    Vec3D sampleColor = CalculateHitColor(scatteredRay, depth - 1);

    Vec3D scatterColor = (scatterPayload.damping * scatteringPDF * sampleColor) / valPDF
            / M_PI * std::max(0., dot(hitPayload.hitNormal, -light_.GetDirection())) * 3.;
    return scatterColor;
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

bool RT::Scene::RayTrace(RT::Ray &ray, RT::HitPayload& payload) {
    // Find all bounding boxes which ray intersects
    std::vector<std::pair<int, RT::BoundingBox*>> orderedBoxes;
    for (auto& pBox : boundingBoxes_){
        RT::HitPayload boundingPayload;
        for (const auto boxObject : pBox.GetBoundingBox()){
            ray.RayIntersect(boxObject, boundingPayload);
        }
        if (boundingPayload.hitDist != DBL_MAX){
            std::pair<int, RT::BoundingBox*> pair = std::make_pair(boundingPayload.hitDist, &pBox);
            orderedBoxes.emplace_back(pair);
        }
    }
    // Find the closest hit of any object inside the bounding boxes
    sort(orderedBoxes.begin(), orderedBoxes.end());
    for (const auto& [hitDist, pBox] : orderedBoxes){
        for (const auto pObject : pBox->GetObjectList()){
            ray.RayIntersect(pObject, payload);
        }
    }
    return payload.hitDist != DBL_MAX;
}

void RT::Scene::CalculateBoundingBoxes() {
    for (auto pObject : pObjectList_){
        auto boundingBox = RT::BoundingBox(pObject);
        boundingBoxes_.emplace_back(boundingBox);
    }
}

std::ofstream RT::Scene::CreateBmpFile() const {
    std::ofstream image;
    image.open("output.bmp", std::ios::binary | std::ios::out);
    const int paddingAmount = ((4 - (sceneWidth_ * 3) % 4) % 4);
    const int fileHeaderSize = 14;
    const int informationHeaderSize = 40;
    const int fileSize = fileHeaderSize + informationHeaderSize + sceneWidth_ * sceneHeight_ * 3 + paddingAmount * sceneHeight_;

    unsigned char fileHeader[fileHeaderSize];
    for (int i = 0; i <= 13; i++) {
        fileHeader[i] = 0;
    }
    fileHeader[0] = 'B';
    fileHeader[1] = 'M';
    fileHeader[2] = fileSize;
    fileHeader[3] = fileSize >> 8;
    fileHeader[4] = fileSize >> 16;
    fileHeader[5] = fileSize >> 24;
    fileHeader[10] = fileHeaderSize + informationHeaderSize;

    unsigned char informationHeader[informationHeaderSize];
    for (int i = 0; i <= 39; i++) {
        informationHeader[i] = 0;
    }
    informationHeader[0] = informationHeaderSize;
    informationHeader[4] = sceneWidth_;
    informationHeader[5] = sceneWidth_ >> 8;
    informationHeader[6] = sceneWidth_ >> 16;
    informationHeader[7] = sceneWidth_ >> 24;
    informationHeader[8] = sceneHeight_;
    informationHeader[9] = sceneHeight_ >> 8;
    informationHeader[10] = sceneHeight_ >> 16;
    informationHeader[11] = sceneHeight_ >> 24;
    informationHeader[12] = 1;
    informationHeader[14] = 24;

    image.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
    image.write(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

    return image;

}

void RT::Scene::WriteColor(Vec3D color, std::ofstream &image) {
    unsigned char r = static_cast<unsigned char>((int)color[0]);
    unsigned char g = static_cast<unsigned char>((int)color[1]);
    unsigned char b = static_cast<unsigned char>((int)color[2]);
    unsigned char whole[] = { r, g, b };
    image.write(reinterpret_cast<char*>(whole), 3);
}

void RT::Scene::ForTriangleRasterization(Vec3D A, Vec3D B, Vec3D C, Vec3D normal, std::vector<double> &depthBuffer) {
    RT::HitPayload payloadA, payloadB, payloadC;
    RT::Ray rayA(A, camera_.GetPos());
    RT::Ray rayB(B, camera_.GetPos());
    RT::Ray rayC(C, camera_.GetPos());
    for (auto pScreen : rasterScreen_){
        rayA.RayIntersect(pScreen, payloadA);
        rayB.RayIntersect(pScreen, payloadB);
        rayC.RayIntersect(pScreen, payloadC);
    }
    if (payloadA.hitDist != DBL_MAX || payloadB.hitDist != DBL_MAX || payloadC.hitDist != DBL_MAX){
        Vec3D screenA = A + payloadA.hitDist * rayA.GetDirection();
        Vec3D screenB = B + payloadB.hitDist * rayB.GetDirection();
        Vec3D screenC = C + payloadC.hitDist * rayC.GetDirection();
        Vec3D lowerLeft = camera_.GetCenter() - 0.5 * (camera_.GetScreenU() + camera_.GetScreenV());
        Vec3D upperRight = camera_.GetCenter() + 0.5 * (camera_.GetScreenU() + camera_.GetScreenV());
        double minX = std::min(screenA[0], std::min(screenB[0], screenC[0]));
        double maxX = std::max(screenA[0], std::max(screenB[0], screenC[0]));
        double minY = std::min(screenA[2], std::min(screenB[2], screenC[2]));
        double maxY = std::max(screenA[2], std::max(screenB[2], screenC[2]));

        int minI, maxI;
        if (lowerLeft[0] < upperRight[0]){
            minI = std::max((int)floor((minX - lowerLeft[0]) / (upperRight[0] - lowerLeft[0]) * sceneWidth_), 0);
            maxI = std::min((int)ceil((maxX - lowerLeft[0]) / (upperRight[0] - lowerLeft[0]) * sceneWidth_), (int)(sceneWidth_ - 1));
        } else{
            minI = std::min((int)ceil((maxX - lowerLeft[0]) / (upperRight[0] - lowerLeft[0]) * sceneWidth_), (int)(sceneWidth_ - 1));
            maxI = std::max((int)floor((minX - lowerLeft[0]) / (upperRight[0] - lowerLeft[0]) * sceneWidth_) ,0);
        }
        int minJ, maxJ;
        if (lowerLeft[2] < upperRight[2]) {
            minJ = std::max((int) floor((minY - lowerLeft[2]) / (upperRight[2] - lowerLeft[2]) * sceneHeight_), 0);
            maxJ = std::min((int) ceil((maxY - lowerLeft[2]) / (upperRight[2] - lowerLeft[2]) * sceneHeight_),
                                (int) (sceneHeight_ - 1));
        } else{
            maxJ = std::max((int) floor((minY - lowerLeft[2]) / (upperRight[2] - lowerLeft[2]) * sceneHeight_), 0);
            minJ = std::min((int) ceil((maxY - lowerLeft[2]) / (upperRight[2] - lowerLeft[2]) * sceneHeight_),
                                (int) (sceneHeight_ - 1));
        }
        std::cout << minI << " " << maxI << " " << minJ << " " << maxJ << std::endl;
        for (int i = std::min(minI, maxJ); i < std::max(minI, maxI); ++i){
            for (int j = std::min(minJ, maxJ); j < std::max(minJ, maxJ); ++j){
                Vec3D hitColor = Utils::BASE_ALBEDO / M_PI * std::max(0., dot(normal, -light_.GetDirection())) * 3.;
                Vec3D pixelColor = ConvertToRGB(hitColor);
                SetPixelColor(i, j, pixelColor);

            }
        }
    }
}


