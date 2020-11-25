#include "input.hpp"
#include "component/velocity.hpp"
#include "component/player.hpp"
#include <glm/common.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/norm.hpp>
#include <SDL.h>

namespace destroid::input_system {

void update(entt::registry& registry, const unsigned char* keystate)
{
    const auto view = registry.view<Player, Velocity>();

    view.each([&](const Player& player, Velocity& velocity) {
        if (keystate[SDL_SCANCODE_A]) {
            velocity.angular -= player.rotationSpeed;
        }

        if (keystate[SDL_SCANCODE_D]) {
            velocity.angular += player.rotationSpeed;
        }

        if (keystate[SDL_SCANCODE_W]) {
            // Keep the rotation bound between 0 and 360 degrees.
            const float rotation = glm::mod((glm::mod(velocity.angular, 360.F) + 360.F), 360.F);

            velocity.linear.x += glm::sin(glm::radians(rotation)) * player.movementSpeed;
            velocity.linear.y += glm::cos(glm::radians(rotation)) * player.movementSpeed;
        }
    });
}

} // namespace destroid::input_system
