#include "collision.hpp"
#include "entity/tags.hpp"
#include "entity/component/collider.hpp"

namespace destroid::collision_system {

void update(entt::registry& registry)
{
    const auto asteroids = registry.view<Collider, AsteroidTag>();
    const auto players = registry.view<Collider, PlayerTag>();

    for (const auto& asteroid : asteroids) {
        const auto& asteroidCollider = asteroids.get<Collider>(asteroid);

        for (const auto& player : players) {
            const auto& playerCollider = players.get<Collider>(player);

            if (playerCollider.bounds.intersects(asteroidCollider.bounds)) {
                registry.destroy(player);
                registry.destroy(asteroid);
            }
        }
    }
}

} // namespace destroid::collision_system
