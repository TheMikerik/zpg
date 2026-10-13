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

// Dynamic transformation: returns the transformation for a given time (in seconds)
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

    // Pass nullptr to make the object static again
    void setDynamicTransformation(DynamicTransformation dynamic);
    void update(float time);

    // Adds any transformation node (leaf or group) to the composite of this object
    void addNode(std::shared_ptr<TransformNode> node);

private:
    std::shared_ptr<Model> model;
    std::shared_ptr<ShaderProgram> shaderProgram;

    std::shared_ptr<Transformation> transformation;  // static part, edited by setters and keys
    std::shared_ptr<Transformation> dynamicPart;     // refreshed every frame in update()
    DynamicTransformation dynamicTransformation;

    // Composite of everything above; the model matrix is taken from here
    TransformGroup modelTransform;
};

#endif // DRAWABLEOBJECT_H
