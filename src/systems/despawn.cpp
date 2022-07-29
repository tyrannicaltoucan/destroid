#include "despawn.hpp"
#include "entity/components/spawner.hpp"
#include "entity/components/tag.hpp"
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
