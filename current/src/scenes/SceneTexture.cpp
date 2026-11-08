#include "SceneTexture.h"
#include "PositionNormalUVModel.h"
#include "Texture.h"
#include "Light.h"
#include "../../models/plain_uv.h"
#include <memory>

namespace {

const int TEXTURE_UNIT = 0;
const float PLAIN_SCALE = 1.5f;
const float PLAIN_TILT_DEGREES = 60.0f;
const glm::vec3 LIGHT_POSITION(0.0f, 2.0f, 4.0f);
const Light SCENE_LIGHT(LIGHT_POSITION);

}

Scene SceneTexture::create() {
    std::vector<float> plainVertices(std::begin(plainUV), std::end(plainUV));

    Scene scene;
    scene.drawables.emplace_back(PositionNormalUVModel(plainVertices),
                                 ShaderProgram("./shaders/vertex_shader_light.glsl", "./shaders/fragment_shader_light.glsl"));
    DrawableObject& plane = scene.drawables.back();
    plane.texture.reset(new Texture("./images/grass.png"));
    plane.transformation.rotate(PLAIN_TILT_DEGREES, glm::vec3(1.0f, 0.0f, 0.0f));
    plane.transformation.scale(glm::vec3(PLAIN_SCALE));
    plane.updateUniforms = [](const ShaderProgram& shader) {
        shader.setUniform("normalFromAttr", 1);
        shader.setUniform("useTexture", 1);
        shader.setUniform("textureUnitID", TEXTURE_UNIT);
        SCENE_LIGHT.sendTo(shader);
    };

    return scene;
}
