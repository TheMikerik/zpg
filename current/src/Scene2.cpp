#include "Scene2.h"
#include "Model.h"
#include "DrawableObject.h"
#include "Transformation.h"
#include "../models/sphere.h"
#include <memory>
#include <glm/glm.hpp>

std::shared_ptr<Scene> Scene2::create()
{
    auto scene = std::make_shared<Scene>();
    auto shader = std::make_shared<ShaderProgram>("./shaders/vertex_shader.glsl", "./shaders/fragment_shader_red.glsl");

    std::vector<float> sphereVertices(std::begin(sphere), std::end(sphere));
    auto sphereModel = std::make_shared<Model>(sphereVertices);

    auto sphereDrawable = std::make_shared<DrawableObject>(sphereModel, shader);

    Transformation sphereTrans;
    sphereTrans.scale(glm::vec3(0.4f));
    sphereDrawable->setTransformation(sphereTrans);

    sphereDrawable->setDynamicTransformation([](float time) {
        Transformation spin;
        spin.rotate(time * 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        return spin;
    });

    scene->addDrawable(sphereDrawable);

    return scene;
}
