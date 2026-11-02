#include "SceneNightForest.h"
#include "PositionNormalModel.h"
#include "PositionColorModel.h"
#include "../../models/tree.h"
#include "../../models/plain.h"
#include "../../models/sphere.h"
#include "Light.h"

namespace {

const glm::vec3 NIGHT_SKY(0.02f, 0.03f, 0.08f);
const glm::vec3 SOIL_COLOR(0.12f, 0.09f, 0.06f);
const glm::vec3 TREE_COLOR(0.1f, 0.3f, 0.1f);
const float GROUND_SCALE = 5.0f;
const float TREE_SCALE = 0.12f;
const float TREE_SPACING = 1.5f;
const int TREE_RANGE = 2;

const glm::vec3 FIREFLY_POSITION(0.75f, 0.6f, 0.75f);
const glm::vec3 FIREFLY_BODY_COLOR(1.0f, 1.0f, 0.4f);
const float FIREFLY_SCALE = 0.03f;
const glm::vec3 FIREFLY_ATTENUATION(1.0f, 0.7f, 1.8f); // I = 1 / (1 + 0.7 d + 1.8 d^2)
const Light FIREFLY_LIGHT(FIREFLY_POSITION, FIREFLY_ATTENUATION);

void addObject(Scene& scene, const std::vector<float>& vertices, const glm::vec3& color,
               const glm::vec3& position, float scale) {
    scene.drawables.emplace_back(PositionNormalModel(vertices),
                                 ShaderProgram("./shaders/vertex_shader_light.glsl", "./shaders/fragment_shader_light.glsl"));
    DrawableObject& object = scene.drawables.back();
    object.transformation.translate(position);
    object.transformation.scale(glm::vec3(scale));
    object.updateUniforms = [color](const ShaderProgram& shader) {
        shader.setUniform("objectColor", color);
        shader.setUniform("normalFromAttr", 1);
        FIREFLY_LIGHT.sendTo(shader);
    };
}

}

Scene SceneNightForest::create() {
    std::vector<float> plainVertices(std::begin(plain), std::end(plain));
    std::vector<float> treeVertices(std::begin(tree), std::end(tree));
    std::vector<float> sphereVertices(std::begin(sphere), std::end(sphere));

    Scene scene;
    scene.clearColor = NIGHT_SKY;

    addObject(scene, plainVertices, SOIL_COLOR, glm::vec3(0.0f, -0.01f, 0.0f), GROUND_SCALE);

    for (int i = -TREE_RANGE; i <= TREE_RANGE; ++i) {
        for (int j = -TREE_RANGE; j <= TREE_RANGE; ++j) {
            glm::vec3 position(i * TREE_SPACING, 0.0f, j * TREE_SPACING);
            addObject(scene, treeVertices, TREE_COLOR, position, TREE_SCALE);
        }
    }

    scene.drawables.emplace_back(PositionColorModel(sphereVertices),
                                 ShaderProgram("./shaders/vertex_shader_light.glsl", "./shaders/fragment_shader_light.glsl"));
    DrawableObject& firefly = scene.drawables.back();
    firefly.transformation.translate(FIREFLY_POSITION);
    firefly.transformation.scale(glm::vec3(FIREFLY_SCALE));
    firefly.updateUniforms = [](const ShaderProgram& shader) {
        shader.setUniform("objectColor", FIREFLY_BODY_COLOR);
    };

    return scene;
}
