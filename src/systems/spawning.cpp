#include "spawning.hpp"
#include "entity/components/spawner.hpp"
#include "entity/factory.hpp"
#include "math/random.hpp"
#include "math/rectangle.hpp"
#include <entt/entity/registry.hpp>
#include <glm/vec2.hpp>

namespace destroid::spawning_system {

void update(entt::registry& registry)
{
    const auto view = registry.view<Spawner>();
    view.each([&](auto& spawner) {
        if (spawner.count == spawner.capacity) {
            return;
        }

        const auto viewport = registry.ctx<Rect>();
        const int spawnDireciton = random::between(1, 4);
        glm::vec2 position;

        switch (spawnDireciton) {
        case 1:
            position = {viewport.x, random::between(viewport.y, viewport.height)};
            break;
        case 2:
            position = {viewport.width, random::between(viewport.y, viewport.height)};
            break;
        case 3:
            position = {random::between(viewport.x, viewport.width), viewport.y};
            break;
        case 4:
            position = {random::between(viewport.x, viewport.width), viewport.height};
            break;
        }

        entity::createAsteroid(registry, position);
        spawner.count += 1;
    });
}

} // namespace destroid::spawning_system
