#include "SceneSpecular.h"
#include "PositionNormalModel.h"
#include "../../models/sphere.h"
#include "Transformation.h"
#include "Light.h"

namespace {

const float OBJECT_OFFSET = 0.5f;
const float BALL_RADIUS = 0.4f;
const float SHININESS = 1.0f;
const glm::vec3 OBJECT_COLOR(1.0f, 0.5f, 0.2f);
const glm::vec3 LIGHT_POSITION(0.0f, 0.0f, -2.0f);
const glm::vec3 LIGHT_COLOR(1.0f, 1.0f, 1.0f);
const Light SCENE_LIGHT(LIGHT_POSITION, LIGHT_COLOR);

void addBall(Scene& scene, const std::vector<float>& sphereVertices, const glm::vec3& position,
             const std::string& fragmentShader) {
    scene.drawables.emplace_back(PositionNormalModel(sphereVertices),
                                 ShaderProgram("./shaders/vertex_shader_normal_correct.glsl", fragmentShader));
    DrawableObject& ball = scene.drawables.back();
    ball.transformation.translate(position);
    ball.transformation.scale(glm::vec3(BALL_RADIUS));
    ball.updateUniforms = [](const ShaderProgram& shader) {
        shader.setUniform("objectColor", OBJECT_COLOR);
        shader.setUniform("shininess", SHININESS);
        SCENE_LIGHT.sendTo(shader);
    };
}

}

Scene SceneSpecular::create() {
    std::vector<float> sphereVertices(std::begin(sphere), std::end(sphere));

    Scene scene;
    addBall(scene, sphereVertices, glm::vec3(-OBJECT_OFFSET, 0.0f, 0.0f),
            "./shaders/fragment_shader_specular_wrong.glsl");
    addBall(scene, sphereVertices, glm::vec3(OBJECT_OFFSET, 0.0f, 0.0f),
            "./shaders/fragment_shader_specular_clipped.glsl");

    return scene;
}
