#include "spawning.hpp"
#include "base/random.hpp"
#include "base/rectangle.hpp"
#include "component/spawner.hpp"
#include "entity/factory.hpp"
#include <entt/entity/registry.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec2.hpp>

namespace destroid::spawning_system {

void update(entt::registry& registry)
{
    const auto view = registry.view<Spawner>();
    view.each([&](auto& spawner) {
        if (spawner.count == spawner.capacity) {
            return;
        }

        const auto viewport = registry.ctx<Rectangle>();
        const auto direction = glm::vec2{random::either(-1.F, 1.F), random::either(-1.F, 1.F)};
        const float angle = glm::radians(random::between(0.F, 359.F));

        const auto orientation = glm::vec2{
            glm::sin(angle) * direction.x,
            glm::cos(angle) * direction.y,
        };

        glm::vec2 position;
        const int spawnDireciton = random::between(1, 4);

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

        const float rotation = random::between(0.F, 359.F);
        entity_factory::spawnAsteroid(registry, position, orientation, rotation);

        spawner.count += 1;
    });
}

} // namespace destroid::spawning_system
