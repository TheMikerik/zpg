#pragma once

#include "Model.h"

class PositionNormalUVModel : public Model {
public:
    explicit PositionNormalUVModel(const std::vector<float>& vertices)
        : Model(vertices, 8, 3, 6) {
    }
};
