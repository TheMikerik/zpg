#ifndef SCENE1_H
#define SCENE1_H

#include "Scene.h"
#include "ShaderProgram.h"
#include <memory>

class Scene1 {
public:
    static std::shared_ptr<Scene> create();
};

#endif // SCENE1_H
