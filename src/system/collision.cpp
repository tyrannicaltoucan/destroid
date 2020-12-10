#include "collision.hpp"
#include "component/asteroid.hpp"
#include "component/player.hpp"
#include "component/collider.hpp"

namespace destroid::collision_system {

void update(entt::registry& registry)
{
    const auto asteroids = registry.view<Collider, Asteroid>();
    const auto players = registry.view<Collider, Player>();

    for (const auto& asteroid : asteroids) {
        const auto asteroidBounds = asteroids.get<Collider>(asteroid).bounds;

        for (const auto& player : players) {
            const auto playerBounds = players.get<Collider>(player).bounds;

            if (playerBounds.intersects(asteroidBounds)) {
                registry.destroy(player);
                registry.destroy(asteroid);
            }
        }
    }
}

} // namespace destroid::collision_system
