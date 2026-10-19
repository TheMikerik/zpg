#pragma once

#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>

class Camera; // dopredna deklarace (kvuli krizovemu odkazu)

class ShaderProgram {
public:
    ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
    ShaderProgram(ShaderProgram&& other) noexcept;
    ~ShaderProgram();

    void use() const;
    GLuint getProgramID() const { return programID; }

    void setCamera(Camera* camera);
    void applyCamera() const;

    void setUniform(const std::string& name, float value) const;
    void setUniform(const std::string& name, const glm::vec3& vec) const;
    void setUniform(const std::string& name, const glm::mat4& mat) const;

private:
    GLuint programID;
    Camera* camera = nullptr;

    std::string loadShaderSource(const std::string& filePath) const;
    void compileShader(const std::string& source, GLuint shader, const std::string& shaderType) const;
    void linkProgram(GLuint vertexShader, GLuint fragmentShader);
    GLint getUniformLocation(const std::string& name) const;
};
