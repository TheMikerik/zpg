#include "Scene5.h"
#include "Model.h"
#include "DrawableObject.h"
#include "Transformation.h"
#include "TransformGroup.h"
#include "../models/sun.h"
#include "../models/earth.h"
#include "../models/moon.h"
#include <memory>
#include <glm/glm.hpp>

namespace {

const glm::vec3 Y_AXIS(0.0f, 1.0f, 0.0f);

const float SUN_RADIUS = 0.25f;
const float EARTH_RADIUS = 0.08f;
const float EARTH_ORBIT = 0.6f;
const float MOON_RADIUS = 0.025f;
const float MOON_ORBIT = 0.2f;

std::shared_ptr<Transformation> rotationNode(float degrees)
{
    auto t = std::make_shared<Transformation>();
    t->rotate(degrees, Y_AXIS);
    return t;
}

std::shared_ptr<Transformation> translationNode(float x)
{
    auto t = std::make_shared<Transformation>();
    t->translate(glm::vec3(x, 0.0f, 0.0f));
    return t;
}

std::shared_ptr<Transformation> uniformScaleNode(float s)
{
    auto t = std::make_shared<Transformation>();
    t->scale(glm::vec3(s));
    return t;
}

// Funkce vygenerovana AI
template <size_t N>
std::vector<float> positionAndColor(const float (&data)[N])
{
    const size_t floatsPerVertex = 9;
    std::vector<float> result;
    result.reserve(N / floatsPerVertex * 6);
    for (size_t i = 0; i + floatsPerVertex <= N; i += floatsPerVertex) {
        result.insert(result.end(), data + i, data + i + 6);
    }
    return result;
}

}

std::shared_ptr<Scene> Scene5::create()
{
    auto scene = std::make_shared<Scene>();
    auto shader = std::make_shared<ShaderProgram>("./shaders/vertex_shader.glsl", "./shaders/fragment_shader.glsl");

    std::vector<float> sunVertices = positionAndColor(sun);
    std::vector<float> earthVertices = positionAndColor(earth);
    std::vector<float> moonVertices = positionAndColor(moon);

    auto sunModel = std::make_shared<Model>(sunVertices);
    auto earthModel = std::make_shared<Model>(earthVertices);
    auto moonModel = std::make_shared<Model>(moonVertices);

    auto sunDrawable = std::make_shared<DrawableObject>(sunModel, shader);
    sunDrawable->setTransformation(*uniformScaleNode(SUN_RADIUS));
    sunDrawable->setDynamicTransformation([](float time) {
        auto spin = std::make_shared<TransformGroup>();
        spin->add(rotationNode(time * 20.0f));
        return Transformation(spin->getMatrix());
    });

    
    auto earthDrawable = std::make_shared<DrawableObject>(earthModel, shader);
    earthDrawable->setTransformation(*uniformScaleNode(EARTH_RADIUS));
    earthDrawable->setDynamicTransformation([](float time) {
        auto earthPlace = std::make_shared<TransformGroup>();
        earthPlace->add(rotationNode(time * 360.0f));
        earthPlace->add(translationNode(EARTH_ORBIT));
        earthPlace->add(rotationNode(time * 45.0f));
        return Transformation(earthPlace->getMatrix());
    });

    auto moonDrawable = std::make_shared<DrawableObject>(moonModel, shader);
    moonDrawable->setTransformation(*uniformScaleNode(MOON_RADIUS));
    moonDrawable->setDynamicTransformation([](float time) {
        auto moonPlace = std::make_shared<TransformGroup>();
        moonPlace->add(rotationNode(time * 180.0f));
        moonPlace->add(translationNode(MOON_ORBIT));
        moonPlace->add(rotationNode(time * 360.0f / 27.0f));
        moonPlace->add(translationNode(EARTH_ORBIT));
        moonPlace->add(rotationNode(time * 45.0f));
        return Transformation(moonPlace->getMatrix());
    });

    scene->addDrawable(sunDrawable);
    scene->addDrawable(earthDrawable);
    scene->addDrawable(moonDrawable);

    return scene;
}
