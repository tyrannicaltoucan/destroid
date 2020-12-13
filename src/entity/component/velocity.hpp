#pragma once

#include <glm/vec2.hpp>

namespace destroid {

struct Velocity {
    glm::vec2 linear;
    float angular = 0.F;
};

} // namespace destroid
