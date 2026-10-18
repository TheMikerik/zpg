#pragma once

#include "DrawableObject.h"
#include <vector>

class Camera;

struct Scene {
    std::vector<DrawableObject> drawables;

    void setCamera(Camera* camera);
    void update(float time);
    void render() const;
};
