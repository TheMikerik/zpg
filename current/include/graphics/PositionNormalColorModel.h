#pragma once

#include "Model.h"

class PositionNormalColorModel : public Model {
public:
    explicit PositionNormalColorModel(const std::vector<float>& vertices)
        : Model(vertices, 9, 6) {
    }
};
