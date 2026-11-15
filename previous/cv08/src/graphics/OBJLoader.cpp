// DOCASNE RESENI: Cely tento soubor byl vytvoren AI

#include "OBJLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

std::vector<float> OBJLoader::loadOBJ(const std::string& filePath, bool withUV) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ file: " << filePath << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::vector<Vertex> vertices;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v") {
            float x, y, z;
            iss >> x >> y >> z;
            positions.push_back(glm::vec3(x, y, z));
        }
        else if (type == "vn") {
            float x, y, z;
            iss >> x >> y >> z;
            normals.push_back(glm::normalize(glm::vec3(x, y, z)));
        }
        else if (type == "vt") {
            float u, v;
            iss >> u >> v;
            uvs.push_back(glm::vec2(u, v));
        }
        else if (type == "f") {
            std::vector<std::string> faceVertices;
            std::string vertexStr;
            while (iss >> vertexStr) {
                faceVertices.push_back(vertexStr);
            }

            auto parseVertex = [&](const std::string& vertexStr) -> Vertex {
                Vertex v;
                std::istringstream viss(vertexStr);
                int posIndex = 0, texIndex = 0, normIndex = 0;
                char slash;

                viss >> posIndex;
                if (viss.peek() == '/') {
                    viss >> slash;
                    if (viss.peek() != '/') {
                        viss >> texIndex;
                    }
                    if (viss.peek() == '/') {
                        viss >> slash >> normIndex;
                    }
                }

                v.position = positions[posIndex - 1];
                v.normal = (normIndex > 0 && normIndex <= normals.size())
                    ? normals[normIndex - 1]
                    : glm::vec3(0.0f, 1.0f, 0.0f);

                v.uv = (texIndex > 0 && texIndex <= uvs.size())
                    ? uvs[texIndex - 1]
                    : glm::vec2(0.0f);

                return v;
            };

            for (size_t i = 1; i + 1 < faceVertices.size(); ++i) {
                vertices.push_back(parseVertex(faceVertices[0]));
                vertices.push_back(parseVertex(faceVertices[i]));
                vertices.push_back(parseVertex(faceVertices[i + 1]));
            }
        }
    }

    file.close();
    return createVertexArray(vertices, withUV);
}

std::vector<float> OBJLoader::createVertexArray(const std::vector<Vertex>& vertices, bool withUV) {
    std::vector<float> result;
    result.reserve(vertices.size() * (withUV ? 8 : 6));

    for (const auto& vertex : vertices) {
        result.push_back(vertex.position.x);
        result.push_back(vertex.position.y);
        result.push_back(vertex.position.z);

        result.push_back(vertex.normal.x);
        result.push_back(vertex.normal.y);
        result.push_back(vertex.normal.z);

        if (withUV) {
            result.push_back(vertex.uv.x);
            result.push_back(vertex.uv.y);
        }
    }

    return result;
}
