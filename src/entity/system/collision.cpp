#include "collision.hpp"
#include "entity/components.hpp"

namespace destroid::collision_system {

void update(entt::registry& registry)
{
    const auto asteroids = registry.view<Collider, Asteroid>();
    const auto players = registry.view<Collider, Player>();

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
