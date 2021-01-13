#include "despawn.hpp"
#include "component/spawner.hpp"
#include "entity/tags.hpp"
#include <entt/entity/registry.hpp>

namespace destroid::despawn_system {

void update(entt::registry& registry)
{
    const auto despawnView = registry.view<DespawnTag>();
    registry.destroy(despawnView.begin(), despawnView.end());

    const auto spawnerView = registry.view<Spawner>();
    spawnerView.each([&](auto& spawner) {
        spawner.count = registry.size<AsteroidTag>();
    });
}

} // namespace destroid::despawn_system
