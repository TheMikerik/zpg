#pragma once

#include "Scene.h"
#include "ShaderProgram.h"
#include <memory>

class Scene3 {
public:
    static std::shared_ptr<Scene> create();
};
