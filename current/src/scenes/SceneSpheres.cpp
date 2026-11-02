#include "SceneSpheres.h"
#include "PositionColorModel.h"
#include "../../models/sphere.h"
#include "Transformation.h"
#include "Light.h"

namespace {

const float BALL_RADIUS = 0.2f;
const float BALL_OFFSET = 0.5f;
const glm::vec3 LIGHT_POSITION(0.0f, 0.0f, 0.0f);
const Light SCENE_LIGHT(LIGHT_POSITION);

void addBall(Scene& scene, const std::vector<float>& sphereVertices, const glm::vec3& position, const glm::vec3& color,
             ShadingModel shadingModel, bool lit) {
    scene.drawables.emplace_back(PositionColorModel(sphereVertices),
                                 ShaderProgram("./shaders/vertex_shader_light.glsl", "./shaders/fragment_shader_light.glsl"));
    DrawableObject& ballDrawable = scene.drawables.back();
    ballDrawable.transformation.translate(position);
    ballDrawable.transformation.scale(glm::vec3(BALL_RADIUS));
    ballDrawable.updateUniforms = [color, shadingModel, lit](const ShaderProgram& shader) {
        shader.setUniform("objectColor", color);
        if (lit) {
            shader.setUniform("shadingModel", shadingModel);
            SCENE_LIGHT.sendTo(shader);
        }
    };
}

}

Scene SceneSpheres::create() {
    std::vector<float> sphereVertices(std::begin(sphere), std::end(sphere));

    Scene scene;
    addBall(scene, sphereVertices, glm::vec3( BALL_OFFSET,  BALL_OFFSET, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
            LAMBERT, false);
    addBall(scene, sphereVertices, glm::vec3(-BALL_OFFSET,  BALL_OFFSET, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
            LAMBERT, true);
    addBall(scene, sphereVertices, glm::vec3( BALL_OFFSET, -BALL_OFFSET, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),
            PHONG, true);
    addBall(scene, sphereVertices, glm::vec3(-BALL_OFFSET, -BALL_OFFSET, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f),
            BLINN, true);

    return scene;
}
