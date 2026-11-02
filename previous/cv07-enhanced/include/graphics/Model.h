#pragma once

#include <vector>
#include <GL/glew.h>

class Model {
public:
    Model(Model&& other) noexcept;
    ~Model();

    void bind() const;
    void unbind() const;
    GLsizei getVertexCount() const { return vertexCount; }

protected:
    Model(const std::vector<float>& vertices, GLsizei floatsPerVertex, GLsizei colorOffset);

private:
    GLuint VAO;
    GLuint VBO;
    GLsizei vertexCount;
};
