#pragma once

#include "Model.h"
#include "Texture.h"
#include "Cubemap.h"
#include "ShaderProgram.h"
#include "Transformation.h"
#include "TransformNode.h"
#include <functional>
#include <memory>
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
    std::shared_ptr<Texture> texture;
    std::shared_ptr<Cubemap> cubemap; // skybox

    Transformation transformation;
    Transformation dynamicPart;
    glm::mat4 nodeMatrix = glm::mat4(1.0f);

    DynamicTransformation dynamic;
    UniformUpdate updateUniforms;
};
