#include "Scene3.h"
#include "../models/bushes.h"
#include "../models/tree.h"
#include "../models/sphere.h"
#include "../models/clock.h"
#include "../models/plain.h"
#include "Transformation.h"
#include "TransformGroup.h"
#include <glm/gtc/constants.hpp>
#include <ctime>
#include <random>

namespace {

const float FOREST_RANGE = 4.0f;
const int TREE_COUNT = 80;
const float SOIL_COLOR[3] = {0.35f, 0.22f, 0.10f};

float randomInRange(std::mt19937& gen, float min, float max)
{
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

std::vector<float> groundPositionAndColor()
{
    std::vector<float> vertices(std::begin(plain), std::end(plain));
    for (size_t i = 0; i < vertices.size(); i += 6) {
        for (int c = 0; c < 3; ++c) {
            vertices[i + 3 + c] = SOIL_COLOR[c];
        }
    }
    return vertices;
}

glm::vec3 randomLeafColor(std::mt19937& gen)
{
    return glm::vec3(randomInRange(gen, 0.1f, 0.3f),
                     randomInRange(gen, 0.45f, 0.8f),
                     randomInRange(gen, 0.1f, 0.25f));
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

void addClock(Scene& scene, const std::vector<float>& clockVertices, const glm::vec3& position)
{
    scene.drawables.emplace_back(Model(clockVertices, true),
                                 ShaderProgram("./shaders/vertex_shader_clock.glsl", "./shaders/fragment_shader.glsl"));
    DrawableObject& clockDrawable = scene.drawables.back();
    clockDrawable.transformation.translate(position);
    clockDrawable.transformation.rotate(180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    clockDrawable.transformation.scale(glm::vec3(-0.35f, 0.35f, 0.35f));
    clockDrawable.updateUniforms = setClockHands;
}

}

Scene Scene3::create()
{
    std::vector<float> groundVertices = groundPositionAndColor();
    std::vector<float> bushesVertices(std::begin(bushes), std::end(bushes));
    std::vector<float> treeVertices(std::begin(tree), std::end(tree));
    std::vector<float> sphereVertices(std::begin(sphere), std::end(sphere));
    std::vector<float> clockVertices(std::begin(clockModel), std::end(clockModel));

    std::random_device rd;
    std::mt19937 gen(rd());

    Scene scene;

    scene.drawables.emplace_back(Model(groundVertices),
                                 ShaderProgram("./shaders/vertex_shader.glsl", "./shaders/fragment_shader.glsl"));
    DrawableObject& groundDrawable = scene.drawables.back();
    groundDrawable.transformation.translate(glm::vec3(0.0f, -0.01f, 0.0f));
    groundDrawable.transformation.scale(glm::vec3(FOREST_RANGE + 1.0f, 1.0f, FOREST_RANGE + 1.0f));

    for (int i = 0; i < 12; ++i) {
        scene.drawables.emplace_back(Model(bushesVertices),
                                     ShaderProgram("./shaders/vertex_shader.glsl", "./shaders/fragment_shader_green.glsl"));
        addRandomInstance(scene.drawables.back(), gen, 0.12f, 0.2f);
    }

    for (int i = 0; i < TREE_COUNT; ++i) {
        scene.drawables.emplace_back(Model(treeVertices),
                                     ShaderProgram("./shaders/vertex_shader_tree.glsl", "./shaders/fragment_shader_tree.glsl"));
        DrawableObject& treeDrawable = scene.drawables.back();
        addRandomInstance(treeDrawable, gen, 0.08f, 0.14f);

        glm::vec3 leafColor = randomLeafColor(gen);
        treeDrawable.updateUniforms = [leafColor](const ShaderProgram& shader) {
            shader.setUniform("leafColor", leafColor);
        };
    }

    scene.drawables.emplace_back(Model(sphereVertices),
                                 ShaderProgram("./shaders/vertex_shader.glsl", "./shaders/fragment_shader_red.glsl"));
    DrawableObject& sunDrawable = scene.drawables.back();
    sunDrawable.transformation.translate(glm::vec3(0.7f, 0.7f, 0.0f));
    sunDrawable.transformation.scale(glm::vec3(0.15f));

    addClock(scene, clockVertices, glm::vec3(0.0f, 1.0f, -1.5f));
    addClock(scene, clockVertices, glm::vec3(2.5f, 1.0f, 1.5f));

    return scene;
}
