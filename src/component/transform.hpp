#pragma once

#include <glm/vec2.hpp>

namespace destroid {

struct Transform {
    glm::vec2 position;
    float rotation = 0.0f;
};

} // namespace destroid
