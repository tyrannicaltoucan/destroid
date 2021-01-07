#pragma once

#include <entt/entity/fwd.hpp>

namespace destroid::input_system {

void update(entt::registry& registry, const unsigned char* keystate, float delta);

} // namespace destroid::input_system
