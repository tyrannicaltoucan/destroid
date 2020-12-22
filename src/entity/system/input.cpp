#include "input.hpp"
#include "entity/tags.hpp"
#include "entity/component/body.hpp"
#include "entity/component/ship.hpp"
#include "entity/component/transform.hpp"
#include "entity/component/weapon.hpp"
#include <glm/trigonometric.hpp>
#include <SDL.h>

namespace destroid::input_system {

void update(entt::registry& registry, const unsigned char* keystate)
{
    const auto entities = registry.view<Body, Ship, Transform, Weapon>();

    for (const auto& entity : entities) {
        const auto [body, ship, transform, weapon] = entities.get<Body, Ship, Transform, Weapon>(entity);

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

        if (keystate[SDL_SCANCODE_SPACE]) {
            if (weapon.cooldown <= 0.F) {
                weapon.cooldown = 0.F;
                weapon.canFire = true;
            }
        }
    }
}

} // namespace destroid::input_system
