#include "physics.hpp"
#include "math/rectangle.hpp"
#include "entity/components.hpp"
#include <glm/common.hpp>

namespace destroid::physics_system {

void update(entt::registry& registry, float delta)
{
    const auto view = registry.view<Velocity, Transform, Collider>();

    for (const auto& entity : view) {
        const auto [velocity, transform, collider] = view.get<Velocity, Transform, Collider>(entity);

        transform.position += velocity.linear * delta;
        transform.rotation = velocity.angular;

        collider.bounds.x = transform.position.x;
        collider.bounds.y = transform.position.y;

        // Keep rotations bound between 0-360 degrees.
        velocity.angular = glm::mod((glm::mod(velocity.angular, 360.F) + 360.F), 360.F);

        // Wrap entities around the viewport if they pass its edges.
        const auto& viewport = registry.ctx<Rectangle>();

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
    }
}

} // namespace destroid::physics_system
