#pragma once

#include "Model.h"

class PositionNormalModel : public Model {
public:
    explicit PositionNormalModel(const std::vector<float>& vertices)
        : Model(vertices, 6, 3) {
    }
};
