#include "Scene5.h"
#include "../models/sun.h"
#include "../models/earth.h"
#include "../models/moon.h"
#include "Transformation.h"
#include "TransformGroup.h"

namespace {

const glm::vec3 Y_AXIS(0.0f, 1.0f, 0.0f);

const float SUN_RADIUS = 0.25f;
const float EARTH_RADIUS = 0.08f;
const float EARTH_ORBIT = 0.6f;
const float MOON_RADIUS = 0.025f;
const float MOON_ORBIT = 0.2f;

Transformation rotationNode(float degrees)
{
    Transformation t;
    t.rotate(degrees, Y_AXIS);
    return t;
}

Transformation translationNode(float x)
{
    Transformation t;
    t.translate(glm::vec3(x, 0.0f, 0.0f));
    return t;
}

Transformation uniformScaleNode(float s)
{
    Transformation t;
    t.scale(glm::vec3(s));
    return t;
}

// Funkce vygenerovana AI
template <size_t N>
std::vector<float> positionAndColor(const float (&data)[N])
{
    const size_t floatsPerVertex = 9;
    std::vector<float> result;
    result.reserve(N / floatsPerVertex * 6);
    for (size_t i = 0; i + floatsPerVertex <= N; i += floatsPerVertex) {
        result.insert(result.end(), data + i, data + i + 6);
    }
    return result;
}

}

Scene Scene5::create()
{
    std::vector<float> sunVertices = positionAndColor(sun);
    std::vector<float> earthVertices = positionAndColor(earth);
    std::vector<float> moonVertices = positionAndColor(moon);

    Scene scene;

    scene.drawables.emplace_back(Model(sunVertices),
                                 ShaderProgram("./shaders/vertex_shader.glsl", "./shaders/fragment_shader.glsl"));
    DrawableObject& sunDrawable = scene.drawables.back();
    sunDrawable.transformation = uniformScaleNode(SUN_RADIUS);
    sunDrawable.dynamic = [](float time) {
        return rotationNode(time * 20.0f);
    };

    scene.drawables.emplace_back(Model(earthVertices),
                                 ShaderProgram("./shaders/vertex_shader.glsl", "./shaders/fragment_shader.glsl"));
    DrawableObject& earthDrawable = scene.drawables.back();
    earthDrawable.transformation = uniformScaleNode(EARTH_RADIUS);
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

    scene.drawables.emplace_back(Model(moonVertices),
                                 ShaderProgram("./shaders/vertex_shader.glsl", "./shaders/fragment_shader.glsl"));
    DrawableObject& moonDrawable = scene.drawables.back();
    moonDrawable.transformation = uniformScaleNode(MOON_RADIUS);
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

    return scene;
}
