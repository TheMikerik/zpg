#pragma once

#include "Model.h"
#include "ShaderProgram.h"
#include "Transformation.h"
#include "TransformNode.h"
#include <functional>
#include <glm/glm.hpp>

using DynamicTransformation = std::function<Transformation(float time)>;
using UniformUpdate = std::function<void(const ShaderProgram& shader)>;

struct DrawableObject {
    DrawableObject(Model model, ShaderProgram shader);

    void addNode(const TransformNode& node);

    void update(float time);
    void render() const;

    Model model;
    ShaderProgram shader;

    Transformation transformation;
    Transformation dynamicPart;
    glm::mat4 nodeMatrix = glm::mat4(1.0f);

    DynamicTransformation dynamic;
    UniformUpdate updateUniforms;
};
