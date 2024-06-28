#ifndef MAIN_CPP_OBJECTLOADER_H
#define MAIN_CPP_OBJECTLOADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <exception>

#include "../RayTrace/Objects.h"
#include "../Log.h"

constexpr const char BIG_DELIMITER = ' ';
constexpr const char SMALL_DELIMITER = '/';

class ObjLoader{
public:
    static std::shared_ptr<RT::TriangleMesh> loadTriangleMeshObj(const std::string& filename, bool flipVertices = false);

private:
};


inline std::shared_ptr<RT::TriangleMesh> ObjLoader::loadTriangleMeshObj(const std::string &filename, bool flipVertices) {
    std::ifstream file("../objects/" + filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file: " + filename);
    }

    std::vector<Vec3D> vertices;
    std::vector<Vector<int, 3>> triangles;

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream inputStream(line);
        char bigDelimiter = ' ';
        std::string bigToken;
        std::getline(inputStream, bigToken, bigDelimiter);
        if (bigToken == "v") {
            Vec3D vertex;
            inputStream >> vertex;
            vertices.push_back(vertex);
        } else if (bigToken == "f") {
            std::string smallToken;
            std::vector<int> polygonIndices;
            int vertInd;
            for (size_t i = 0; std::getline(inputStream, bigToken, BIG_DELIMITER); ++i) {
                std::stringstream inputStream2(bigToken);
                std::getline(inputStream2, smallToken, SMALL_DELIMITER);
                vertInd = std::stoi(smallToken) - 1;
                polygonIndices.emplace_back(vertInd);
            }
            if (polygonIndices.size() != 3) {
                throw std::invalid_argument(".obj file should contain only triangular faces.");
            } else{
                Vector<int, 3> triangle{polygonIndices[0], polygonIndices[1], polygonIndices[2]};
                triangles.emplace_back(triangle);
            }
        }
    }
    if (flipVertices) {
        for (auto &triangle: triangles) {
            int a = triangle[0];
            int b = triangle[1];
            triangle[0] = b;
            triangle[1] = a;
        }
    }
    std::shared_ptr<RT::TriangleMesh> triangleMesh = std::make_shared<RT::TriangleMesh>(RT::TriangleMesh(vertices, triangles));
    triangleMesh->updateEdgesAndNormals();
    return triangleMesh;
}

#endif
