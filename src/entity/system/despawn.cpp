#include "despawn.hpp"
#include "entity/component/lifetime.hpp"

namespace destroid::despawn_system {

void update(entt::registry& registry, float delta)
{
    const auto view = registry.view<Lifetime>();

    for (const auto& entity : view) {
        auto& lifetime = view.get<Lifetime>(entity);

        lifetime.duration -= delta;

        if (lifetime.duration <= 0.F) {
            registry.destroy(entity);
        }
    }
}

} // namespace destroid::despawn_system
