#pragma once

#include "graphics/renderer.hpp"
#include <entt/entity/registry.hpp>

namespace destroid::drawing_system {

void update(entt::registry& registry, Renderer& renderer);

} // namespace destroid::drawing_system
