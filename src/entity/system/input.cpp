#include "input.hpp"
#include "entity/tags.hpp"
#include "entity/component/body.hpp"
#include "entity/component/ship.hpp"
#include "entity/component/transform.hpp"
#include <glm/trigonometric.hpp>
#include <SDL.h>

namespace destroid::input_system {

void update(entt::registry& registry, const unsigned char* keystate)
{
    const auto entities = registry.view<Body, Ship, Transform>();

    for (const auto& entity : entities) {
        const auto [body, ship, transform] = entities.get<Body, Ship, Transform>(entity);

        if (keystate[SDL_SCANCODE_A]) {
            transform.rotation -= ship.rotationSpeed;
        }

        if (keystate[SDL_SCANCODE_D]) {
            transform.rotation += ship.rotationSpeed;
        }

        if (keystate[SDL_SCANCODE_W]) {
            const float rotationRads = glm::radians(transform.rotation);
            body.velocity.x += glm::sin(rotationRads) * ship.movementSpeed;
            body.velocity.y += glm::cos(rotationRads) * ship.movementSpeed;
        }

        // Add damping to make controls feel "tighter".
        body.velocity *= body.drag;
    }
}

} // namespace destroid::input_system
