#pragma once

#include "TransformNode.h"
#include <vector>
class TransformGroup : public TransformNode {
public:
    void add(const TransformNode& child);
    void clear();

    glm::mat4 getMatrix() const override;

private:
    std::vector<const TransformNode*> children;
};
