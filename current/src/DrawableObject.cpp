#include "DrawableObject.h"
#include <utility>

DrawableObject::DrawableObject(Model model, ShaderProgram shader)
    : model(std::move(model)), shader(std::move(shader))
{
}

void DrawableObject::addNode(const TransformNode& node)
{
    nodeMatrix = node.getMatrix() * nodeMatrix;
}

void DrawableObject::update(float time)
{
    dynamicPart = dynamic ? dynamic(time) : Transformation();
}

void DrawableObject::render() const
{
    shader.use();

    glm::mat4 modelMatrix = nodeMatrix * dynamicPart.getMatrix() * transformation.getMatrix();
    shader.setUniform("modelMatrix", modelMatrix);

    model.bind();
    glDrawArrays(GL_TRIANGLES, 0, model.getVertexCount());
    model.unbind();
}
