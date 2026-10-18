#pragma once

#include "DrawableObject.h"
#include <vector>

struct Scene {
    std::vector<DrawableObject> drawables;

    void update(float time);
    void render() const;
};
