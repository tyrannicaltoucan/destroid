#include "input.hpp"
#include "entity/components.hpp"
#include <glm/trigonometric.hpp>
#include <SDL.h>

namespace destroid::input_system {

void update(entt::registry& registry, const unsigned char* keystate)
{
    const auto entities = registry.view<Player, Velocity>();

    for (const auto& entity : entities) {
        const auto [player, velocity] = entities.get<Player, Velocity>(entity);

        if (keystate[SDL_SCANCODE_A]) {
            velocity.angular -= player.rotationSpeed;
        }

        if (keystate[SDL_SCANCODE_D]) {
            velocity.angular += player.rotationSpeed;
        }

        if (keystate[SDL_SCANCODE_W]) {
            velocity.linear.x += glm::sin(glm::radians(velocity.angular)) * player.movementSpeed;
            velocity.linear.y += glm::cos(glm::radians(velocity.angular)) * player.movementSpeed;
        }

        // Add damping to make controls feel "tighter".
        velocity.linear *= player.damping;
    }
}

} // namespace destroid::input_system
