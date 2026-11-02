#include "Scene2.h"
#include "../models/sphere.h"
#include "Transformation.h"

Scene Scene2::create()
{
    std::vector<float> sphereVertices(std::begin(sphere), std::end(sphere));

    Scene scene;
    scene.drawables.emplace_back(Model(sphereVertices),
                                 ShaderProgram("./shaders/vertex_shader.glsl", "./shaders/fragment_shader_red.glsl"));

    DrawableObject& sphereDrawable = scene.drawables.back();
    sphereDrawable.transformation.scale(glm::vec3(0.4f));
    sphereDrawable.dynamic = [](float time) {
        Transformation spin;
        spin.rotate(time * 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        return spin;
    };

    return scene;
}
