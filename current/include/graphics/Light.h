#pragma once

#include "ShaderProgram.h"
#include <glm/glm.hpp>

// Svetlo ve scene. Zatim jen bodove svetlo (pozice + barva).
// Dalsi typy svetel (smerove, reflektor) se pridaji sem.
class Light {
public:
    Light(const glm::vec3& position, const glm::vec3& color);

    // Posle pozici a barvu svetla do shaderu (uniformy lightPos a lightColor)
    void sendTo(const ShaderProgram& shader) const;

private:
    glm::vec3 position;
    glm::vec3 color;
};
