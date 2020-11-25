#pragma once

#include "graphics/texture.hpp"
#include "utility/rectangle.hpp"
#include <memory>

namespace destroid {

struct Drawable {
    std::shared_ptr<Texture> texture = nullptr;
    Rectangle region{ 0.F, 0.F, 0.F, 0.F };
};

}
