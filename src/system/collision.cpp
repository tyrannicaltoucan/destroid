#include "collision.hpp"
#include "entity/tags.hpp"
#include "component/collider.hpp"

namespace destroid::collision_system {

void update(entt::registry& registry)
{
    const auto asteroids = registry.view<Collider, AsteroidTag>();
    const auto players = registry.view<Collider, PlayerTag>();
    const auto bullets = registry.view<Collider, BulletTag>();

    for (const auto& asteroid : asteroids) {
        const auto& asteroidCollider = asteroids.get<Collider>(asteroid);

        for (const auto& projectile : bullets) {
            const auto& projectileCollider = bullets.get<Collider>(projectile);

            if (projectileCollider.bounds.intersects(asteroidCollider.bounds)) {
                registry.destroy(projectile);

                if (registry.valid(asteroid)) {
                    registry.destroy(asteroid);
                }
            }
        }

        for (const auto& player : players) {
            const auto& playerCollider = players.get<Collider>(player);

            if (playerCollider.bounds.intersects(asteroidCollider.bounds)) {
                registry.destroy(player);

                if (registry.valid(asteroid)) {
                    registry.destroy(asteroid);
                }
            }
        }
    }
}

} // namespace destroid::collision_system
