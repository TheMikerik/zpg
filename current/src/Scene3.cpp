#include "Scene3.h"
#include "../models/bushes.h"
#include "../models/tree.h"
#include "../models/sphere.h"
#include "../models/clock.h"
#include "Transformation.h"
#include "TransformGroup.h"
#include <glm/gtc/constants.hpp>
#include <ctime>
#include <random>

namespace {

const float FOREST_RANGE = 4.0f;

float randomInRange(std::mt19937& gen, float min, float max)
{
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

void setClockHands(const ShaderProgram& shader)
{
    std::time_t now = std::time(nullptr);
    std::tm local = *std::localtime(&now);
    float seconds = static_cast<float>(local.tm_sec);
    float minutes = local.tm_min + seconds / 60.0f;
    float hours = (local.tm_hour % 12) + minutes / 60.0f;

    shader.setUniform("hourAngle", hours / 12.0f * glm::two_pi<float>());
    shader.setUniform("minuteAngle", minutes / 60.0f * glm::two_pi<float>());
    shader.setUniform("secondAngle", seconds / 60.0f * glm::two_pi<float>());
}

void addRandomInstance(DrawableObject& drawable, std::mt19937& gen, float scaleMin, float scaleMax)
{
    Transformation scale;
    scale.scale(glm::vec3(randomInRange(gen, scaleMin, scaleMax)));

    Transformation rotation;
    rotation.rotate(randomInRange(gen, 0.0f, 360.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    float x = randomInRange(gen, -FOREST_RANGE, FOREST_RANGE);
    float z = randomInRange(gen, -FOREST_RANGE, FOREST_RANGE);
    Transformation position;
    position.translate(glm::vec3(x, 0.0f, z));

    TransformGroup instance;
    instance.add(scale);
    instance.add(rotation);
    instance.add(position);
    drawable.addNode(instance);
}

}

Scene Scene3::create()
{
    std::vector<float> bushesVertices(std::begin(bushes), std::end(bushes));
    std::vector<float> treeVertices(std::begin(tree), std::end(tree));
    std::vector<float> sphereVertices(std::begin(sphere), std::end(sphere));

    std::random_device rd;
    std::mt19937 gen(rd());

    Scene scene;

    for (int i = 0; i < 12; ++i) {
        scene.drawables.emplace_back(Model(bushesVertices),
                                     ShaderProgram("./shaders/vertex_shader.glsl", "./shaders/fragment_shader_green.glsl"));
        addRandomInstance(scene.drawables.back(), gen, 0.12f, 0.2f);
    }

    for (int i = 0; i < 15; ++i) {
        scene.drawables.emplace_back(Model(treeVertices),
                                     ShaderProgram("./shaders/vertex_shader.glsl", "./shaders/fragment_shader_purple.glsl"));
        addRandomInstance(scene.drawables.back(), gen, 0.08f, 0.14f);
    }

    scene.drawables.emplace_back(Model(sphereVertices),
                                 ShaderProgram("./shaders/vertex_shader.glsl", "./shaders/fragment_shader_red.glsl"));
    DrawableObject& sunDrawable = scene.drawables.back();
    sunDrawable.transformation.translate(glm::vec3(0.7f, 0.7f, 0.0f));
    sunDrawable.transformation.scale(glm::vec3(0.15f));

    std::vector<float> clockVertices(std::begin(clockModel), std::end(clockModel));
    scene.drawables.emplace_back(Model(clockVertices, true),
                                 ShaderProgram("./shaders/vertex_shader_clock.glsl", "./shaders/fragment_shader.glsl"));
    DrawableObject& clockDrawable = scene.drawables.back();
    clockDrawable.transformation.translate(glm::vec3(0.0f, 1.0f, -1.5f));
    clockDrawable.transformation.rotate(180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    clockDrawable.transformation.scale(glm::vec3(-0.35f, 0.35f, 0.35f));
    clockDrawable.updateUniforms = setClockHands;

    return scene;
}
