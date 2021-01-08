#include "collision.hpp"
#include "entity/tags.hpp"
#include "component/collider.hpp"
#include <entt/entity/registry.hpp>

namespace destroid::collision_system {

void update(entt::registry& registry)
{
    const auto asteroids = registry.view<Collider, AsteroidTag>();
    const auto players = registry.view<Collider, PlayerTag>();
    const auto bullets = registry.view<Collider, BulletTag>();

    asteroids.each([&](const auto& asteroid, const auto& asteroidCollider) {
        // Check collisions between asteroids and bullets
        bullets.each([&](const auto& projectile, const auto& bulletCollider) {
            if (bulletCollider.bounds.intersects(asteroidCollider.bounds)) {
                if (registry.valid(asteroid)) {
                    registry.destroy(asteroid);
                }

                registry.destroy(projectile);
            }
        });

        // Check collisions between asteroids and players
        players.each([&](const auto& player, const auto& playerCollider) {
            if (playerCollider.bounds.intersects(asteroidCollider.bounds)) {
                if (registry.valid(asteroid)) {
                    registry.destroy(asteroid);
                }

                registry.destroy(player);
            }
        });
    });
}

} // namespace destroid::collision_system
