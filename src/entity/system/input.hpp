#pragma once

#include <entt/entity/registry.hpp>

namespace destroid::input_system {

void update(entt::registry& registry, const unsigned char* keystate);

} // namespace destroid::input_system
