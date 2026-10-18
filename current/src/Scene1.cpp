#include "Scene1.h"
#include "../models/triangle.h"
#include "Transformation.h"
#include "TransformGroup.h"

Scene Scene1::create()
{
    std::vector<float> triangleVertices(std::begin(triangle), std::end(triangle));

    Transformation baseTrans;
    baseTrans.scale(glm::vec3(0.3f));

    Transformation rotation;
    rotation.rotate(45.0f, glm::vec3(0.0f, 0.0f, 1.0f));

    Transformation translation;
    translation.translate(glm::vec3(0.5f, 0.0f, 0.0f));

    TransformGroup orderA;
    orderA.add(baseTrans);
    orderA.add(rotation);
    orderA.add(translation);

    TransformGroup orderB;
    orderB.add(baseTrans);
    orderB.add(translation);
    orderB.add(rotation);

    Scene scene;

    scene.drawables.emplace_back(Model(triangleVertices),
                                 ShaderProgram("./shaders/vertex_shader.glsl", "./shaders/fragment_shader.glsl"));
    scene.drawables.back().addNode(orderA);

    scene.drawables.emplace_back(Model(triangleVertices),
                                 ShaderProgram("./shaders/vertex_shader.glsl", "./shaders/fragment_shader.glsl"));
    scene.drawables.back().addNode(orderB);

    return scene;
}
