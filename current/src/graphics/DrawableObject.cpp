#include "DrawableObject.h"
#include <utility>

DrawableObject::DrawableObject(Model model, ShaderProgram shader)
    : model(std::move(model)), shader(std::move(shader)) {
}

void DrawableObject::addNode(const TransformNode& node) {
    nodeMatrix = node.getMatrix() * nodeMatrix;
}

void DrawableObject::update(float time) {
    dynamicPart = dynamic ? dynamic(time) : Transformation();
}

void DrawableObject::render() const {
    shader.use();
    if (texture) {
        texture->bind(0);
    }
    if (cubemap) {
        cubemap->bind(0);
        glDepthMask(GL_FALSE);
    }
    if (updateUniforms) {
        updateUniforms(shader);
    }

    glm::mat4 modelMatrix = nodeMatrix * dynamicPart.getMatrix() * transformation.getMatrix();
    if (!cubemap) {
        shader.setUniform("modelMatrix", modelMatrix);
    }
    shader.applyCamera();

    model.bind();
    glDrawArrays(GL_TRIANGLES, 0, model.getVertexCount());
    model.unbind();
    glDepthMask(GL_TRUE);
}
