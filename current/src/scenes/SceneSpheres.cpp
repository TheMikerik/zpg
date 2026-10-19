#include "SceneSpheres.h"
#include "../../models/sphere.h"
#include "Transformation.h"

namespace {

const float BALL_RADIUS = 0.2f;
const float BALL_OFFSET = 0.5f;

void addBall(Scene& scene, const std::vector<float>& sphereVertices, const glm::vec3& position, const glm::vec3& color)
{
    scene.drawables.emplace_back(Model(sphereVertices),
                                 ShaderProgram("./shaders/vertex_shader.glsl", "./shaders/fragment_shader_color.glsl"));
    DrawableObject& ballDrawable = scene.drawables.back();
    ballDrawable.transformation.translate(position);
    ballDrawable.transformation.scale(glm::vec3(BALL_RADIUS));
    ballDrawable.updateUniforms = [color](const ShaderProgram& shader) {
        shader.setUniform("objectColor", color);
    };
}

}

Scene SceneSpheres::create()
{
    std::vector<float> sphereVertices(std::begin(sphere), std::end(sphere));

    Scene scene;
    addBall(scene, sphereVertices, glm::vec3( BALL_OFFSET,  BALL_OFFSET, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    addBall(scene, sphereVertices, glm::vec3(-BALL_OFFSET,  BALL_OFFSET, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    addBall(scene, sphereVertices, glm::vec3( BALL_OFFSET, -BALL_OFFSET, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    addBall(scene, sphereVertices, glm::vec3(-BALL_OFFSET, -BALL_OFFSET, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f));

    return scene;
}
