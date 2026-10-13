// TransformNode.h
#pragma once

#include <glm/glm.hpp>

class TransformNode {
public:
    virtual ~TransformNode() = default;

    virtual glm::mat4 getMatrix() const = 0;
};
