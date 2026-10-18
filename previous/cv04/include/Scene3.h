#ifndef SCENE3_H
#define SCENE3_H

#include "Scene.h"
#include "ShaderProgram.h"
#include <memory>

class Scene3 {
public:
    static std::shared_ptr<Scene> create();
};

#endif // SCENE3_H
