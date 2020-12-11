#pragma once

#include "graphics/texture.hpp"
#include "math/rectangle.hpp"
#include <memory>

namespace destroid {

struct Drawable {
    std::shared_ptr<Texture> texture = nullptr;
    Rectangle region;
};

} // namespace destroid
