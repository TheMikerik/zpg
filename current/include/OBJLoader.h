#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

class OBJLoader {
public:
    static std::vector<float> loadOBJ(const std::string& filePath);

private:
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
    };

    static std::vector<float> createVertexArray(const std::vector<Vertex>& vertices);
};
