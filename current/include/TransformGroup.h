// TransformGroup.h
#pragma once

#include "TransformNode.h"
#include <memory>
#include <vector>

class TransformGroup : public TransformNode {
public:
    void add(std::shared_ptr<TransformNode> child);
    void clear();

    glm::mat4 getMatrix() const override;

private:
    std::vector<std::shared_ptr<TransformNode>> children;
};
