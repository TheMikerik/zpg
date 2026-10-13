#include "Scene4.h"
#include "Model.h"
#include "DrawableObject.h"
#include "Transformation.h"
#include "OBJLoader.h"
#include <memory>
#include <glm/glm.hpp>

std::shared_ptr<Scene> Scene4::create()
{
    auto scene = std::make_shared<Scene>();
    auto shader = std::make_shared<ShaderProgram>("./shaders/vertex_shader.glsl", "./shaders/fragment_shader.glsl");

    std::vector<float> loginVertices = OBJLoader::loadOBJ("./models/login.obj");
    auto loginModel = std::make_shared<Model>(loginVertices);

    auto loginDrawable = std::make_shared<DrawableObject>(loginModel, shader);

    Transformation loginTrans;
    loginTrans.scale(glm::vec3(0.01f));
    loginDrawable->setTransformation(loginTrans);

    scene->addDrawable(loginDrawable);

    return scene;
}
