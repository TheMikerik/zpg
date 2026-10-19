#include "Light.h"

Light::Light(const glm::vec3& position, const glm::vec3& color)
    : position(position), color(color)
{
}

void Light::sendTo(const ShaderProgram& shader) const
{
    shader.setUniform("lightPos", position);
    shader.setUniform("lightColor", color);
}
