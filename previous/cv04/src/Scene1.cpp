#include "Scene1.h"
#include "Model.h"
#include "DrawableObject.h"
#include "Transformation.h"
#include "../models/triangle.h"
#include <memory>
#include <glm/glm.hpp>

std::shared_ptr<Scene> Scene1::create()
{
    auto scene = std::make_shared<Scene>();
    auto shader = std::make_shared<ShaderProgram>("./shaders/vertex_shader.glsl", "./shaders/fragment_shader.glsl");

    std::vector<float> triangleVertices(std::begin(triangle), std::end(triangle));
    auto triangleModel = std::make_shared<Model>(triangleVertices);

    auto baseTrans = std::make_shared<Transformation>();
    baseTrans->scale(glm::vec3(0.3f));

    auto rotation = std::make_shared<Transformation>();
    rotation->rotate(45.0f, glm::vec3(0.0f, 0.0f, 1.0f));

    auto translation = std::make_shared<Transformation>();
    translation->translate(glm::vec3(0.5f, 0.0f, 0.0f));

    // M = T * R * S
    auto orderA = std::make_shared<TransformGroup>();
    orderA->add(baseTrans);
    orderA->add(rotation);
    orderA->add(translation);

    // M = R * T * S
    auto orderB = std::make_shared<TransformGroup>();
    orderB->add(baseTrans);
    orderB->add(translation);
    orderB->add(rotation);

    auto rotatedFirst = std::make_shared<DrawableObject>(triangleModel, shader);
    rotatedFirst->addNode(orderA);

    auto translatedFirst = std::make_shared<DrawableObject>(triangleModel, shader);
    translatedFirst->addNode(orderB);

    scene->addDrawable(rotatedFirst);
    scene->addDrawable(translatedFirst);

    return scene;
}
