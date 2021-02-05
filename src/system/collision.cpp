#include "collision.hpp"
#include "component/collider.hpp"
#include <entt/core/type_traits.hpp>
#include <entt/entity/registry.hpp>

namespace destroid::collision_system {

namespace {

    using namespace entt::literals;
    using DespawnTag = entt::tag<"despawnable"_hs>;

} // namespace

void update(entt::registry& registry)
{
    const auto asteroids = registry.view<Collider, entt::tag<"asteroid"_hs>>();
    const auto players = registry.view<Collider, entt::tag<"ship"_hs>>();
    const auto bullets = registry.view<Collider, entt::tag<"bullet"_hs>>();

    asteroids.each([&](const auto& asteroid, const auto& asteroidCollider) {
        // Check collisions between asteroids and bullets
        bullets.each([&](const auto& bullet, const auto& bulletCollider) {
            if (bulletCollider.bounds.intersects(asteroidCollider.bounds)) {
                registry.emplace_or_replace<DespawnTag>(asteroid);
                registry.emplace_or_replace<DespawnTag>(bullet);
            }
        });

        // Check collisions between asteroids and players
        players.each([&](const auto& player, const auto& playerCollider) {
            if (playerCollider.bounds.intersects(asteroidCollider.bounds)) {
                registry.emplace_or_replace<DespawnTag>(asteroid);
                registry.emplace_or_replace<DespawnTag>(player);
            }
        });
    });
}

} // namespace destroid::collision_system
