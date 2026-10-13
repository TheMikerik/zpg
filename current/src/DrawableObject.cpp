#include "DrawableObject.h"

DrawableObject::DrawableObject(std::shared_ptr<Model> model,
                              std::shared_ptr<ShaderProgram> shaderProgram,
                              const Transformation& initialTransformation)
    : model(model), shaderProgram(shaderProgram),
      transformation(std::make_shared<Transformation>(initialTransformation)),
      dynamicPart(std::make_shared<Transformation>())
{
    modelTransform.add(transformation);
    modelTransform.add(dynamicPart);
}

void DrawableObject::render() const
{
    shaderProgram->use();

    glm::mat4 modelMat = modelTransform.getMatrix();
    shaderProgram->setUniform("modelMatrix", modelMat);

    model->bind();
    glDrawArrays(GL_TRIANGLES, 0, model->getVertexCount());
    model->unbind();
}

void DrawableObject::setTransformation(const Transformation& trans)
{
    *transformation = trans;
}

void DrawableObject::setTransformation(const glm::mat4& matrix)
{
    *transformation = Transformation(matrix);
}

void DrawableObject::addTransformation(const Transformation& trans)
{
    *transformation = Transformation(trans.getMatrix() * transformation->getMatrix());
}

Transformation& DrawableObject::getTransformation()
{
    return *transformation;
}

void DrawableObject::setDynamicTransformation(DynamicTransformation dynamic)
{
    dynamicTransformation = dynamic;
    if (dynamicTransformation) {
        update(0.0f);
    } else {
        *dynamicPart = Transformation();
    }
}

void DrawableObject::update(float time)
{
    if (dynamicTransformation) {
        *dynamicPart = dynamicTransformation(time);
    }
}

void DrawableObject::addNode(std::shared_ptr<TransformNode> node)
{
    modelTransform.add(std::move(node));
}
