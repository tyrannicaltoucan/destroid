#include "physics.hpp"
#include "entity/component/body.hpp"
#include "entity/component/collider.hpp"
#include "entity/component/transform.hpp"
#include "math/rectangle.hpp"
#include <glm/common.hpp>

namespace destroid::physics_system {

void update(entt::registry& registry, float delta)
{
    const auto view = registry.view<Body, Transform, Collider>();

    for (const auto& entity : view) {
        const auto [body, transform, collider] = view.get<Body, Transform, Collider>(entity);

        transform.position += body.velocity * delta;
        transform.rotation = glm::mod((glm::mod(transform.rotation, 360.F) + 360.F), 360.F);

        collider.bounds.x = transform.position.x;
        collider.bounds.y = transform.position.y;

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
