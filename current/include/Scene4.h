#ifndef SCENE4_H
#define SCENE4_H

#include "Scene.h"
#include "ShaderProgram.h"
#include <memory>

class Scene4 {
public:
    static std::shared_ptr<Scene> create();
};

#endif // SCENE4_H
