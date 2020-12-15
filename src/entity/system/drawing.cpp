#include "drawing.hpp"
#include "entity/component/drawable.hpp"
#include "entity/component/transform.hpp"

namespace destroid::drawing_system {

void update(entt::registry& registry, Renderer& renderer)
{
    const auto entities = registry.view<Drawable, Transform>();

    for (const auto& entity : entities) {
        const auto [drawable, transform] = entities.get<Drawable, Transform>(entity);
        const float scale = 1.F;

        renderer.draw(
            *drawable.texture,
            drawable.region,
            transform.position,
            transform.rotation,
            scale);
    }
}

} // namespace destroid::drawing_system
