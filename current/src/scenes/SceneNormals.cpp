#include "SceneNormals.h"
#include "PositionNormalModel.h"
#include "../../models/sphere.h"
#include "Transformation.h"
#include "Light.h"

namespace {

const float OBJECT_OFFSET = 0.5f;
const glm::vec3 ELLIPSOID_SCALE(0.3f, 0.1f, 0.15f);
const glm::vec3 OBJECT_COLOR(1.0f, 0.5f, 0.2f);
const glm::vec3 LIGHT_POSITION(0.0f, 1.0f, 1.0f);
const glm::vec3 LIGHT_COLOR(1.0f, 1.0f, 1.0f);
const Light SCENE_LIGHT(LIGHT_POSITION, LIGHT_COLOR);

void addEllipsoid(Scene& scene, const std::vector<float>& sphereVertices, const glm::vec3& position,
                  const std::string& vertexShader) {
    scene.drawables.emplace_back(PositionNormalModel(sphereVertices),
                                 ShaderProgram(vertexShader, "./shaders/fragment_shader_phong.glsl"));
    DrawableObject& ellipsoid = scene.drawables.back();
    ellipsoid.transformation.translate(position);
    ellipsoid.transformation.scale(ELLIPSOID_SCALE);
    ellipsoid.updateUniforms = [](const ShaderProgram& shader) {
        shader.setUniform("objectColor", OBJECT_COLOR);
        SCENE_LIGHT.sendTo(shader);
    };
}

}

Scene SceneNormals::create() {
    std::vector<float> sphereVertices(std::begin(sphere), std::end(sphere));

    Scene scene;
    addEllipsoid(scene, sphereVertices, glm::vec3(-OBJECT_OFFSET, 0.0f, 0.0f),
                 "./shaders/vertex_shader_normal.glsl");
    addEllipsoid(scene, sphereVertices, glm::vec3(OBJECT_OFFSET, 0.0f, 0.0f),
                 "./shaders/vertex_shader_normal_correct.glsl");

    return scene;
}
