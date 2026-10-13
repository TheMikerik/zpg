// TransformGroup.h
#ifndef TRANSFORMGROUP_H
#define TRANSFORMGROUP_H

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

#endif // TRANSFORMGROUP_H
