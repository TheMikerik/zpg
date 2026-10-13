#ifndef SCENE2_H
#define SCENE2_H

#include "Scene.h"
#include "ShaderProgram.h"
#include <memory>

class Scene2 {
public:
    static std::shared_ptr<Scene> create();
};

#endif // SCENE2_H
