#include "Scene4.h"
#include "OBJLoader.h"
#include "Transformation.h"

Scene Scene4::create()
{
    std::vector<float> loginVertices = OBJLoader::loadOBJ("./models/login.obj");

    Scene scene;
    scene.drawables.emplace_back(Model(loginVertices),
                                 ShaderProgram("./shaders/vertex_shader.glsl", "./shaders/fragment_shader.glsl"));
    scene.drawables.back().transformation.scale(glm::vec3(0.01f));

    return scene;
}
