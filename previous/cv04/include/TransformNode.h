// TransformNode.h
#ifndef TRANSFORMNODE_H
#define TRANSFORMNODE_H

#include <glm/glm.hpp>

class TransformNode {
public:
    virtual ~TransformNode() = default;

    virtual glm::mat4 getMatrix() const = 0;
};

#endif // TRANSFORMNODE_H
