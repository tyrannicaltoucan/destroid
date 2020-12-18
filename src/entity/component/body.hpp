#pragma once

#include <glm/vec2.hpp>

namespace destroid {

struct Body {
    float drag = 0.F;
    glm::vec2 velocity;
};

} // namespace destroid
