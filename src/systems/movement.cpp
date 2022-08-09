#include "movement.hpp"
#include "entity/components/collider.hpp"
#include "entity/components/drag.hpp"
#include "entity/components/momentum.hpp"
#include "entity/components/transform.hpp"
#include "math/rectangle.hpp"
#include <entt/entity/registry.hpp>
#include <glm/common.hpp>
#include <glm/exponential.hpp>

namespace destroid::movement_system {
namespace {

void wrapPosition(const Rect& viewport, glm::vec2& position)
{
    if (position.x < viewport.left()) {
        position.x += viewport.width;
    }

    if (position.x > viewport.right()) {
        position.x -= viewport.width;
    }

    if (position.y < viewport.top()) {
        position.y += viewport.height;
    }

    if (position.y > viewport.bottom()) {
        position.y -= viewport.height;
    }
}

} // namespace

void update(entt::registry& registry, float delta)
{
    const auto momentumView = registry.view<Momentum, Transform>();
    momentumView.each([&](const auto& momentum, auto& transform) {
        transform.position += momentum.linear * delta;
        transform.rotation += momentum.angular * delta;

        // Keep rotations bound between 0 and 360 degrees.
        transform.rotation = glm::mod((glm::mod(transform.rotation, 360.0f) + 360.0f), 360.0f);

        const auto viewport = registry.ctx<Rect>();
        wrapPosition(viewport, transform.position);
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
