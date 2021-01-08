#include "movement.hpp"
#include "component/collider.hpp"
#include "component/drag.hpp"
#include "component/momentum.hpp"
#include "component/transform.hpp"
#include "base/rectangle.hpp"
#include <entt/entity/registry.hpp>
#include <glm/common.hpp>
#include <glm/exponential.hpp>

namespace destroid::movement_system {

void update(entt::registry& registry, float delta)
{
    const auto momentumView = registry.view<Momentum, Transform>();
    momentumView.each([&](const auto& momentum, auto& transform) {
        transform.position += momentum.linear * delta;
        transform.rotation += momentum.angular * delta;

        // Keep rotations bound between 0 and 360 degrees.
        transform.rotation = glm::mod((glm::mod(transform.rotation, 360.F) + 360.F), 360.F);

        // Keep entity positions inside the window area.
        const auto viewport = registry.ctx<Rectangle>();

        if (transform.position.x <= viewport.left()) {
            transform.position.x = transform.position.x + viewport.width;
        }

        if (transform.position.x >= viewport.right()) {
            transform.position.x = transform.position.x - viewport.width;
        }

        if (transform.position.y <= viewport.top()) {
            transform.position.y = transform.position.y + viewport.height;
        }

        if (transform.position.y >= viewport.bottom()) {
            transform.position.y = transform.position.y - viewport.height;
        }
    });

    const auto colliderView = registry.view<Collider, Transform>();
    colliderView.each([](auto& collider, const auto& transform) {
        collider.bounds.x = transform.position.x;
        collider.bounds.y = transform.position.y;
    });

    const auto dragView = registry.view<Drag, Momentum>();
    dragView.each([&](const auto& drag, auto& momentum) {
        momentum.linear *= glm::pow(drag.linear, delta);
        momentum.angular *= glm::pow(drag.angular, delta);
    });
}

} // namespace destroid::movement_system
