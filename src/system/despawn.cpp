#include "despawn.hpp"
#include "entity/tags.hpp"
#include <entt/entity/registry.hpp>

namespace destroid::despawn_system {

void update(entt::registry& registry)
{
    const auto view = registry.view<DespawnTag>();
    registry.destroy(view.begin(), view.end());
}

} // namespace destroid::despawn_system
