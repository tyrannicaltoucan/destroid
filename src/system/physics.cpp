#include "physics.hpp"
#include "math/rectangle.hpp"
#include "component/velocity.hpp"
#include "component/player.hpp"
#include "component/transform.hpp"
#include <glm/common.hpp>

namespace destroid::physics_system {

void update(entt::registry& registry, float delta)
{
    const auto view = registry.view<Velocity, Transform>();
    view.each([&](const Velocity& velocity, Transform& transform) {
        transform.position += velocity.linear * delta;
        // Keep the rotation bound between 0 and 360 degrees.
        transform.rotation = glm::mod((glm::mod(velocity.angular, 360.F) + 360.F), 360.F);

        const Rectangle viewRect = registry.ctx<Rectangle>();

        // Wrap entity positions when the apporach the view's edges
        if (transform.position.x <= viewRect.left()) {
            transform.position.x = transform.position.x + viewRect.width;
        }

        if (transform.position.x >= viewRect.right()) {
            transform.position.x = transform.position.x - viewRect.width;
        }

        if (transform.position.y <= viewRect.top()) {
            transform.position.y = transform.position.y + viewRect.height;
        }

        if (transform.position.y >= viewRect.bottom()) {
            transform.position.y = transform.position.y - viewRect.height;
        }
    });
}

} // namespace destroid::physics_system
