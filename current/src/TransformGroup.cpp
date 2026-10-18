// TransformGroup.cpp
#include "TransformGroup.h"

void TransformGroup::add(const TransformNode& child)
{
    children.push_back(&child);
}

void TransformGroup::clear()
{
    children.clear();
}

glm::mat4 TransformGroup::getMatrix() const
{
    glm::mat4 result(1.0f);
    for (const TransformNode* child : children) {
        result = child->getMatrix() * result;
    }
    return result;
}
