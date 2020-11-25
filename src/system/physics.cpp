#include "physics.hpp"
#include "utility/rectangle.hpp"
#include "component/velocity.hpp"
#include "component/player.hpp"
#include "component/transform.hpp"
#include <glm/exponential.hpp>

namespace destroid::physics_system {

void update(entt::registry& registry, float delta)
{
    const auto positionView = registry.view<Velocity, Transform>();
    positionView.each([&](const Velocity& velocity, Transform& transform) {
        transform.position += velocity.linear * delta;
        transform.rotation += velocity.angular * delta;

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

    // Space may not have friction, but adding some makes movement feel nicer!
    const auto dampingView = registry.view<Player, Velocity>();
    dampingView.each([&](const Player& player, Velocity& velocity) {
        velocity.linear *= player.damping;
    });
}

} // namespace destroid::physics_system
