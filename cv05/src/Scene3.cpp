#include "Scene3.h"
#include "Model.h"
#include "DrawableObject.h"
#include "Transformation.h"
#include "TransformGroup.h"
#include "../models/bushes.h"
#include "../models/tree.h"
#include "../models/sphere.h"
#include <memory>
#include <random>
#include <glm/glm.hpp>

namespace {

// tato funkce byla vygenerovana AI
float randomInRange(std::mt19937& gen, float min, float max)
{
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

std::shared_ptr<TransformGroup> randomInstance(std::mt19937& gen,
                                               float yMin, float yMax,
                                               float scaleMin, float scaleMax)
{
    auto scale = std::make_shared<Transformation>();
    scale->scale(glm::vec3(randomInRange(gen, scaleMin, scaleMax)));

    auto rotation = std::make_shared<Transformation>();
    rotation->rotate(randomInRange(gen, 0.0f, 360.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    auto position = std::make_shared<Transformation>();
    position->translate(glm::vec3(randomInRange(gen, -1.0f, 1.0f),
                                  randomInRange(gen, yMin, yMax),
                                  0.0f));

    auto group = std::make_shared<TransformGroup>();
    group->add(scale);
    group->add(rotation);
    group->add(position);
    return group;
}

}

std::shared_ptr<Scene> Scene3::create()
{
    auto scene = std::make_shared<Scene>();

    auto shaderBushes = std::make_shared<ShaderProgram>("./shaders/vertex_shader.glsl", "./shaders/fragment_shader_green.glsl");
    auto shaderTrees = std::make_shared<ShaderProgram>("./shaders/vertex_shader.glsl", "./shaders/fragment_shader_purple.glsl");
    auto shaderSun = std::make_shared<ShaderProgram>("./shaders/vertex_shader.glsl", "./shaders/fragment_shader_red.glsl");

    std::vector<float> bushesVertices(std::begin(bushes), std::end(bushes));
    std::vector<float> treeVertices(std::begin(tree), std::end(tree));

    auto bushesModel = std::make_shared<Model>(bushesVertices);
    auto treeModel = std::make_shared<Model>(treeVertices);

    std::random_device rd;
    std::mt19937 gen(rd());

    for (int i = 0; i < 12; ++i) {
        auto bushesDrawable = std::make_shared<DrawableObject>(bushesModel, shaderBushes);
        bushesDrawable->addNode(randomInstance(gen, -0.9f, -0.7f, 0.12f, 0.2f));
        scene->addDrawable(bushesDrawable);
    }

    for (int i = 0; i < 15; ++i) {
        auto treeDrawable = std::make_shared<DrawableObject>(treeModel, shaderTrees);
        treeDrawable->addNode(randomInstance(gen, -0.65f, -0.45f, 0.08f, 0.14f));
        scene->addDrawable(treeDrawable);
    }

    std::vector<float> sphereVertices(std::begin(sphere), std::end(sphere));
    auto sphereModel = std::make_shared<Model>(sphereVertices);

    auto sunDrawable = std::make_shared<DrawableObject>(sphereModel, shaderSun);
    Transformation sunTrans;
    sunTrans.translate(glm::vec3(0.7f, 0.7f, 0.0f));
    sunTrans.scale(glm::vec3(0.15f));
    sunDrawable->setTransformation(sunTrans);
    scene->addDrawable(sunDrawable);

    return scene;
}
