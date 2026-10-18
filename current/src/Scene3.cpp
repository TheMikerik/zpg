#include "Scene3.h"
#include "../models/bushes.h"
#include "../models/tree.h"
#include "../models/sphere.h"
#include "Transformation.h"
#include "TransformGroup.h"
#include <random>

namespace {

// tato funkce byla vygenerovana AI
float randomInRange(std::mt19937& gen, float min, float max)
{
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

void addRandomInstance(DrawableObject& drawable, std::mt19937& gen,
                       float yMin, float yMax, float scaleMin, float scaleMax)
{
    Transformation scale;
    scale.scale(glm::vec3(randomInRange(gen, scaleMin, scaleMax)));

    Transformation rotation;
    rotation.rotate(randomInRange(gen, 0.0f, 360.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    float x = randomInRange(gen, -1.0f, 1.0f);
    float y = randomInRange(gen, yMin, yMax);
    Transformation position;
    position.translate(glm::vec3(x, y, 0.0f));

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
        addRandomInstance(scene.drawables.back(), gen, -0.9f, -0.7f, 0.12f, 0.2f);
    }

    for (int i = 0; i < 15; ++i) {
        scene.drawables.emplace_back(Model(treeVertices),
                                     ShaderProgram("./shaders/vertex_shader.glsl", "./shaders/fragment_shader_purple.glsl"));
        addRandomInstance(scene.drawables.back(), gen, -0.65f, -0.45f, 0.08f, 0.14f);
    }

    scene.drawables.emplace_back(Model(sphereVertices),
                                 ShaderProgram("./shaders/vertex_shader.glsl", "./shaders/fragment_shader_red.glsl"));
    DrawableObject& sunDrawable = scene.drawables.back();
    sunDrawable.transformation.translate(glm::vec3(0.7f, 0.7f, 0.0f));
    sunDrawable.transformation.scale(glm::vec3(0.15f));

    return scene;
}
