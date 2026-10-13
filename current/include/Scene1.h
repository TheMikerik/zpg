#pragma once

#include "Scene.h"
#include "ShaderProgram.h"
#include <memory>

class Scene1 {
public:
    static std::shared_ptr<Scene> create();
};
