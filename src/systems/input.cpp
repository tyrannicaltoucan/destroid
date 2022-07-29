#include "input.hpp"
#include "entity/components/momentum.hpp"
#include "entity/components/thrust.hpp"
#include "entity/components/transform.hpp"
#include "entity/components/weapon.hpp"
#include <SDL_scancode.h>
#include <entt/entity/registry.hpp>
#include <glm/trigonometric.hpp>

namespace destroid::input_system {

void update(entt::registry& registry, const unsigned char* keystate, float delta)
{
    const auto view = registry.view<Momentum, Thrust, Transform, Weapon>();
    view.each([&](auto& momentum, const auto& thrust, const auto& transform, auto& weapon) {
        if (keystate[SDL_SCANCODE_W]) {
            const float rotationRads = glm::radians(transform.rotation);
            momentum.linear.x += -glm::sin(rotationRads) * thrust.linear * delta;
            momentum.linear.y += glm::cos(rotationRads) * thrust.linear * delta;
        }

        if (keystate[SDL_SCANCODE_A]) {
            momentum.angular += thrust.angular * delta;
        }

        if (keystate[SDL_SCANCODE_D]) {
            momentum.angular += -thrust.angular * delta;
        }

        if (keystate[SDL_SCANCODE_SPACE]) {
            if (weapon.cooldown <= 0.0f) {
                weapon.cooldown = 0.0f;
                weapon.canFire = true;
            }
        }
    });
}

} // namespace destroid::input_system
