#pragma once

#include <glm/glm.hpp>

class ShaderProgram; // dopredna deklarace (kvuli krizovemu odkazu)

class Camera {
public:
    Camera(float aspectRatio);

    void rotate(float deltaAlpha, float deltaFi);
    void moveForward(float amount);
    void moveRight(float amount);
    void setAspectRatio(float aspectRatio);
    void sendMatricesTo(const ShaderProgram& shader) const;

    const glm::mat4& getViewMatrix() const { return viewMatrix; }
    const glm::mat4& getProjectionMatrix() const { return projectionMatrix; }

private:
    glm::vec3 eye;
    glm::vec3 target;
    glm::vec3 up;
    float alpha; // uhel od osy Y (0 = nahoru, PI = dolu)
    float fi;    // uhel v rovine XZ

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    void updateViewMatrix();
};
