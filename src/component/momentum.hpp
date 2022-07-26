#pragma once

#include <glm/vec2.hpp>

namespace destroid {

struct Momentum {
    glm::vec2 linear {};
    float angular = 0.0f;
};

} // namespace destroid
