#include "Light.h"

Light::Light(const glm::vec3& position, const glm::vec3& attenuation)
    : position(position), attenuation(attenuation) {
}

void Light::sendTo(const ShaderProgram& shader) const {
    shader.setUniform("lightCount", 1);
    shader.setUniform("lightPos", position);
    shader.setUniform("attenuation", attenuation);
}
