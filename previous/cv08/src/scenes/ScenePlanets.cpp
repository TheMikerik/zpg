#include "ScenePlanets.h"
#include "PositionColorModel.h"
#include "../../models/sun.h"
#include "../../models/earth.h"
#include "../../models/moon.h"
#include "Transformation.h"
#include "TransformGroup.h"
#include "Light.h"
#include "OBJLoader.h"
#include "PositionNormalModel.h"

namespace {

const glm::vec3 Y_AXIS(0.0f, 1.0f, 0.0f);
const glm::vec3 Z_AXIS(0.0f, 0.0f, 1.0f);

const float UFO_SCALE = 0.002f;
const glm::vec3 UFO_CENTER(0.0f, 21.3f, 0.0f);
const float UFO_ORBIT_RADIUS = 0.4f;
const float UFO_ORBIT_SPEED = 120.0f;
const glm::vec3 UFO_COLOR(0.6f, 0.6f, 0.6f);

const Light SUN_LIGHT(glm::vec3(0.0f, 0.0f, 0.0f));

void setPlanetUniforms(const ShaderProgram& shader) {
    shader.setUniform("useVertexColor", 1);
    shader.setUniform("shadingModel", PHONG);
    SUN_LIGHT.sendTo(shader);
}

const float SUN_RADIUS = 0.5f;
const float EARTH_RADIUS = 0.16f;
const float EARTH_ORBIT = 1.2f;
const float MOON_RADIUS = 0.05f;
const float MOON_ORBIT = 0.4f;

Transformation rotationNode(float degrees) {
    Transformation t;
    t.rotate(degrees, Y_AXIS);
    return t;
}

Transformation translationNode(float x) {
    Transformation t;
    t.translate(glm::vec3(x, 0.0f, 0.0f));
    return t;
}

Transformation uniformScaleNode(float s) {
    Transformation t;
    t.scale(glm::vec3(s));
    return t;
}

template <size_t N>
std::vector<float> positionAndColor(const float (&data)[N]) {
    const size_t floatsPerVertex = 9;
    std::vector<float> result;
    result.reserve(N / floatsPerVertex * 6);
    for (size_t i = 0; i + floatsPerVertex <= N; i += floatsPerVertex) {
        result.insert(result.end(), data + i, data + i + 6);
    }
    return result;
}

}

Scene ScenePlanets::create() {
    std::vector<float> sunVertices = positionAndColor(sun);
    std::vector<float> earthVertices = positionAndColor(earth);
    std::vector<float> moonVertices = positionAndColor(moon);

    Scene scene;

    scene.drawables.emplace_back(PositionColorModel(sunVertices),
                                 ShaderProgram("./shaders/vertex_shader_light.glsl", "./shaders/fragment_shader_light.glsl"));
    DrawableObject& sunDrawable = scene.drawables.back();
    sunDrawable.transformation = uniformScaleNode(SUN_RADIUS);
    sunDrawable.updateUniforms = [](const ShaderProgram& shader) {
        shader.setUniform("useVertexColor", 1);
    };
    sunDrawable.dynamic = [](float time) {
        return rotationNode(time * 20.0f);
    };

    scene.drawables.emplace_back(PositionColorModel(earthVertices),
                                 ShaderProgram("./shaders/vertex_shader_light.glsl", "./shaders/fragment_shader_light.glsl"));
    DrawableObject& earthDrawable = scene.drawables.back();
    earthDrawable.transformation = uniformScaleNode(EARTH_RADIUS);
    earthDrawable.updateUniforms = setPlanetUniforms;
    earthDrawable.dynamic = [](float time) {
        Transformation spin = rotationNode(time * 360.0f);
        Transformation orbit = translationNode(EARTH_ORBIT);
        Transformation sunSpin = rotationNode(time * 45.0f);

        TransformGroup earthPlace;
        earthPlace.add(spin);
        earthPlace.add(orbit);
        earthPlace.add(sunSpin);
        return Transformation(earthPlace.getMatrix());
    };

    scene.drawables.emplace_back(PositionColorModel(moonVertices),
                                 ShaderProgram("./shaders/vertex_shader_light.glsl", "./shaders/fragment_shader_light.glsl"));
    DrawableObject& moonDrawable = scene.drawables.back();
    moonDrawable.transformation = uniformScaleNode(MOON_RADIUS);
    moonDrawable.updateUniforms = setPlanetUniforms;
    moonDrawable.dynamic = [](float time) {
        Transformation spin = rotationNode(time * 180.0f);
        Transformation moonOrbit = translationNode(MOON_ORBIT);
        Transformation moonSpin = rotationNode(time * 360.0f / 27.0f);
        Transformation earthOrbit = translationNode(EARTH_ORBIT);
        Transformation sunSpin = rotationNode(time * 45.0f);

        TransformGroup moonPlace;
        moonPlace.add(spin);
        moonPlace.add(moonOrbit);
        moonPlace.add(moonSpin);
        moonPlace.add(earthOrbit);
        moonPlace.add(sunSpin);
        return Transformation(moonPlace.getMatrix());
    };

    scene.drawables.emplace_back(PositionColorModel(OBJLoader::loadOBJ("./models/ufo.obj")),
                                 ShaderProgram("./shaders/vertex_shader_light.glsl", "./shaders/fragment_shader_light.glsl"));
    DrawableObject& ufoDrawable = scene.drawables.back();
    ufoDrawable.transformation.scale(glm::vec3(UFO_SCALE));
    ufoDrawable.transformation.translate(-UFO_SCALE * UFO_CENTER);
    ufoDrawable.dynamic = [](float time) {
        Transformation radius = translationNode(UFO_ORBIT_RADIUS);
        Transformation circle;
        circle.rotate(time * UFO_ORBIT_SPEED, Z_AXIS);
        Transformation earthOrbit = translationNode(EARTH_ORBIT);
        Transformation sunSpin = rotationNode(time * 45.0f);

        TransformGroup ufoPlace;
        ufoPlace.add(radius);
        ufoPlace.add(circle);
        ufoPlace.add(earthOrbit);
        ufoPlace.add(sunSpin);
        return Transformation(ufoPlace.getMatrix());
    };
    ufoDrawable.updateUniforms = [](const ShaderProgram& shader) {
        shader.setUniform("objectColor", UFO_COLOR);
        shader.setUniform("normalFromAttr", 1);
        shader.setUniform("shadingModel", PHONG);
        SUN_LIGHT.sendTo(shader);
    };

    return scene;
}
