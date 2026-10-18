#include "Camera.h"
#include "ShaderProgram.h"
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

namespace {

const float FOV_DEGREES = 60.0f;
const float Z_NEAR = 0.1f;
const float Z_FAR = 100.0f;
const float ALPHA_MARGIN = 0.01f;

}

Camera::Camera(float aspectRatio)
    : eye(0.0f, 0.0f, 5.0f), up(0.0f, 1.0f, 0.0f),
      alpha(glm::half_pi<float>()), fi(-glm::half_pi<float>())
{
    setAspectRatio(aspectRatio);
    updateViewMatrix();
}

void Camera::rotate(float deltaAlpha, float deltaFi)
{
    alpha = glm::clamp(alpha + deltaAlpha, ALPHA_MARGIN, glm::pi<float>() - ALPHA_MARGIN);
    fi = std::fmod(fi + deltaFi, 2.0f * glm::pi<float>());
    updateViewMatrix();
}

void Camera::moveForward(float amount)
{
    eye += target * amount;
    updateViewMatrix();
}

void Camera::moveRight(float amount)
{
    glm::vec3 right = glm::normalize(glm::cross(target, up));
    eye += right * amount;
    updateViewMatrix();
}

void Camera::setAspectRatio(float aspectRatio)
{
    projectionMatrix = glm::perspective(glm::radians(FOV_DEGREES), aspectRatio, Z_NEAR, Z_FAR);
}

void Camera::sendMatricesTo(const ShaderProgram& shader) const
{
    shader.setUniform("viewMatrix", viewMatrix);
    shader.setUniform("projectionMatrix", projectionMatrix);
}

void Camera::updateViewMatrix()
{
    target.x = std::sin(alpha) * std::cos(fi);
    target.z = std::sin(alpha) * std::sin(fi);
    target.y = std::cos(alpha);

    viewMatrix = glm::lookAt(eye, eye + target, up);
}
