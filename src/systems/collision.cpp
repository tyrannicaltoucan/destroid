#include "collision.hpp"
#include "entity/components/collider.hpp"
#include "entity/components/tag.hpp"
#include "math/circle.hpp"
#include <entt/entity/registry.hpp>

namespace destroid::collision_system {

void update(entt::registry& registry)
{
    const auto asteroids = registry.view<Collider, AsteroidTag>();
    const auto players = registry.view<Collider, ShipTag>();
    const auto bullets = registry.view<Collider, BulletTag>();

    asteroids.each([&](const auto& asteroid, const auto& asteroidCollider) {
        bullets.each([&](const auto& bullet, const auto& bulletCollider) {
            if (bulletCollider.bounds.intersects(asteroidCollider.bounds)) {
                registry.emplace_or_replace<DespawnTag>(asteroid);
                registry.emplace_or_replace<DespawnTag>(bullet);
            }
        });

        players.each([&](const auto& player, const auto& playerCollider) {
            if (playerCollider.bounds.intersects(asteroidCollider.bounds)) {
                registry.emplace_or_replace<DespawnTag>(asteroid);
                registry.emplace_or_replace<DespawnTag>(player);
            }
        });
    });
}

} // namespace destroid::collision_system
