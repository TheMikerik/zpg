#include "SceneSpheres.h"
#include "PositionColorModel.h"
#include "../../models/sphere.h"
#include "Transformation.h"
#include "Light.h"

namespace {

const float BALL_RADIUS = 0.2f;
const float BALL_OFFSET = 0.5f;
const glm::vec3 LIGHT_POSITION(0.0f, 0.0f, 0.0f);
const glm::vec3 LIGHT_COLOR(1.0f, 1.0f, 1.0f);
const Light SCENE_LIGHT(LIGHT_POSITION, LIGHT_COLOR);

void sendAmbientLight(const ShaderProgram& shader) {
    shader.setUniform("lightColor", LIGHT_COLOR);
}

void sendPointLight(const ShaderProgram& shader) {
    SCENE_LIGHT.sendTo(shader);
}

void addBall(Scene& scene, const std::vector<float>& sphereVertices, const glm::vec3& position, const glm::vec3& color,
             const std::string& fragmentShader, const UniformUpdate& sendLight) {
    scene.drawables.emplace_back(PositionColorModel(sphereVertices),
                                 ShaderProgram("./shaders/vertex_shader_light.glsl", fragmentShader));
    DrawableObject& ballDrawable = scene.drawables.back();
    ballDrawable.transformation.translate(position);
    ballDrawable.transformation.scale(glm::vec3(BALL_RADIUS));
    ballDrawable.updateUniforms = [color, sendLight](const ShaderProgram& shader) {
        shader.setUniform("objectColor", color);
        sendLight(shader);
    };
}

}

Scene SceneSpheres::create() {
    std::vector<float> sphereVertices(std::begin(sphere), std::end(sphere));

    Scene scene;
    addBall(scene, sphereVertices, glm::vec3( BALL_OFFSET,  BALL_OFFSET, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
            "./shaders/fragment_shader_ambient.glsl", sendAmbientLight);
    addBall(scene, sphereVertices, glm::vec3(-BALL_OFFSET,  BALL_OFFSET, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
            "./shaders/fragment_shader_light.glsl", sendPointLight);
    addBall(scene, sphereVertices, glm::vec3( BALL_OFFSET, -BALL_OFFSET, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),
            "./shaders/fragment_shader_phong.glsl", sendPointLight);
    addBall(scene, sphereVertices, glm::vec3(-BALL_OFFSET, -BALL_OFFSET, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f),
            "./shaders/fragment_shader_blinn.glsl", sendPointLight);

    return scene;
}
