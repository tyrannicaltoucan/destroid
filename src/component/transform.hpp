#pragma once

#include <glm/vec2.hpp>

namespace destroid {

struct Transform {
    glm::vec2 position{ 0.F, 0.F };
    float rotation = 0.F;
};

} // namespace destroid
