#pragma once

#include <glm/vec2.hpp>

namespace destroid {

struct Velocity {
    glm::vec2 linear{ 0.F, 0.F };
    float angular = 0.F;
};

} // namespace destroid
