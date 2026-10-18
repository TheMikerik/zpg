// DrawableObject.h
#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

#include "Model.h"
#include "ShaderProgram.h"
#include "Transformation.h"
#include "TransformGroup.h"
#include <memory>
#include <functional>
#include <glm/glm.hpp>

using DynamicTransformation = std::function<Transformation(float time)>;

class DrawableObject {
public:
    DrawableObject(std::shared_ptr<Model> model,
                std::shared_ptr<ShaderProgram> shaderProgram,
                const Transformation& transformation = Transformation());

    void render() const;

    void setTransformation(const Transformation& trans);
    void setTransformation(const glm::mat4& matrix);
    void addTransformation(const Transformation& trans);
    Transformation& getTransformation();

    void setDynamicTransformation(DynamicTransformation dynamic);
    void update(float time);

    void addNode(std::shared_ptr<TransformNode> node);

private:
    std::shared_ptr<Model> model;
    std::shared_ptr<ShaderProgram> shaderProgram;

    std::shared_ptr<Transformation> transformation;
    std::shared_ptr<Transformation> dynamicPart;
    DynamicTransformation dynamicTransformation;

    TransformGroup modelTransform;
};

#endif // DRAWABLEOBJECT_H
