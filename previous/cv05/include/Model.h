#pragma once

#include <vector>
#include <GL/glew.h>

class Model {
public:
    // withNormals = false: vrchol je 6 hodnot (pozice, barva)
    // withNormals = true:  vrchol je 9 hodnot (pozice, normála, barva)
    Model(const std::vector<float>& vertices, bool withNormals = false);
    Model(Model&& other) noexcept;
    ~Model();

    void bind() const;
    void unbind() const;
    GLsizei getVertexCount() const { return vertexCount; }

private:
    GLuint VAO;
    GLuint VBO;
    GLsizei vertexCount;
};
