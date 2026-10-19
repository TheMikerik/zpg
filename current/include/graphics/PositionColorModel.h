#pragma once

#include "Model.h"

class PositionColorModel : public Model {
public:
    explicit PositionColorModel(const std::vector<float>& vertices)
        : Model(vertices, 6, 3) {
    }
};
