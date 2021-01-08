#include "despawn.hpp"
#include "component/lifetime.hpp"
#include <entt/entity/registry.hpp>

namespace destroid::despawn_system {

void update(entt::registry& registry, float delta)
{
    const auto view = registry.view<Lifetime>();
    view.each([&](const auto& entity, auto& lifetime) {
        lifetime.duration -= delta;

        if (lifetime.duration <= 0.F) {
            registry.destroy(entity);
        }
    });
}

} // namespace destroid::despawn_system
