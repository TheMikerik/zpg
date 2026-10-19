#include "Model.h"
#include <cstdlib>
#include <iostream>

Model::Model(const std::vector<float>& vertices, GLsizei floatsPerVertex, GLsizei colorOffset)
    : VAO(0), VBO(0), vertexCount(0) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, floatsPerVertex * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, floatsPerVertex * sizeof(float), (void*)(colorOffset * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    vertexCount = static_cast<GLsizei>(vertices.size() / floatsPerVertex);
}

Model::Model(Model&& other) noexcept
    : VAO(other.VAO), VBO(other.VBO), vertexCount(other.vertexCount) {
    other.VAO = 0;
    other.VBO = 0;
}

Model::~Model() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Model::bind() const {
    glBindVertexArray(VAO);
}

void Model::unbind() const {
    glBindVertexArray(0);
}
