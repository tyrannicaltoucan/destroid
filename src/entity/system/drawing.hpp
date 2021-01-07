#pragma once

#include "graphics/renderer.hpp"
#include "graphics/texture.hpp"
#include <entt/entity/registry.hpp>

namespace destroid::drawing_system {

void update(entt::registry& registry, Renderer& renderer, const Texture& texture);

} // namespace destroid::drawing_system
