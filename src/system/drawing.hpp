#pragma once

#include <entt/entity/registry.hpp>

namespace destroid {

class Renderer;
class Texture;

namespace drawing_system {

    void update(entt::registry& registry, Renderer& renderer, const Texture& texture);

} // namespace drawing_system

} // namespace destroid
