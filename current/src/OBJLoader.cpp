// DOCASNE RESENI: Cely tento soubor byl vytvoren AI

#include "OBJLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

std::vector<float> OBJLoader::loadOBJ(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ file: " << filePath << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
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
        else if (type == "f") {
            std::string vertex1, vertex2, vertex3;
            iss >> vertex1 >> vertex2 >> vertex3;

            // Parse vertex format: v, v/vt, v/vt/vn, v//vn
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

                return v;
            };

            Vertex v1 = parseVertex(vertex1);
            Vertex v2 = parseVertex(vertex2);
            Vertex v3 = parseVertex(vertex3);

            vertices.push_back(v1);
            vertices.push_back(v2);
            vertices.push_back(v3);
        }
    }

    file.close();
    return createVertexArray(vertices);
}

std::vector<float> OBJLoader::createVertexArray(const std::vector<Vertex>& vertices)
{
    std::vector<float> result;
    result.reserve(vertices.size() * 6);

    for (const auto& vertex : vertices) {
        result.push_back(vertex.position.x);
        result.push_back(vertex.position.y);
        result.push_back(vertex.position.z);

        result.push_back(vertex.normal.x);
        result.push_back(vertex.normal.y);
        result.push_back(vertex.normal.z);
    }

    return result;
}
