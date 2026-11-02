#pragma once

#include "DrawableObject.h"
#include <vector>

class Camera;

struct Scene {
    std::vector<DrawableObject> drawables;
    glm::vec3 clearColor = glm::vec3(0.59f, 0.76f, 0.92f); // Barva pozadi

    void setCamera(Camera* camera);
    void update(float time);
    void render() const;
};
