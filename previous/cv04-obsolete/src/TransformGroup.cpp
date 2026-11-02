// TransformGroup.cpp
#include "TransformGroup.h"

void TransformGroup::add(std::shared_ptr<TransformNode> child)
{
    children.push_back(std::move(child));
}

void TransformGroup::clear()
{
    children.clear();
}

glm::mat4 TransformGroup::getMatrix() const
{
    glm::mat4 result(1.0f);
    for (const auto& child : children) {
        result = child->getMatrix() * result;
    }
    return result;
}
