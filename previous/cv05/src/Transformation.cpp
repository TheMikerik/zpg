// Transformation.cpp
#include "Transformation.h"
#include <glm/gtc/matrix_transform.hpp>

Transformation::Transformation()
    : modelMatrix(1.0f)
{}

Transformation::Transformation(const glm::mat4& matrix)
    : modelMatrix(matrix)
{}

void Transformation::translate(const glm::vec3& translation){
    modelMatrix = glm::translate(glm::mat4(1.0f), translation) * modelMatrix;
}

void Transformation::rotate(float angleDegrees, const glm::vec3& axis){
    modelMatrix = modelMatrix * glm::rotate(glm::mat4(1.0f), glm::radians(angleDegrees), glm::normalize(axis));
}

void Transformation::scale(const glm::vec3& scaleVec){
    modelMatrix = modelMatrix * glm::scale(glm::mat4(1.0f), scaleVec);
}

const glm::mat4& Transformation::getModelMatrix() const{
    return modelMatrix;
}

glm::mat4 Transformation::getMatrix() const{
    return modelMatrix;
}
