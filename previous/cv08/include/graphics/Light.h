#pragma once

#include "ShaderProgram.h"
#include <glm/glm.hpp>
enum ShadingModel {
    LAMBERT = 0,
    PHONG = 1,
    BLINN = 2
};
class Light {
public:
    explicit Light(const glm::vec3& position, const glm::vec3& attenuation = glm::vec3(1.0f, 0.0f, 0.0f));
    void sendTo(const ShaderProgram& shader) const;

private:
    glm::vec3 position;
    glm::vec3 attenuation;
};
