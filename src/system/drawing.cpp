#include "drawing.hpp"
#include "component/drawable.hpp"
#include "component/transform.hpp"
#include "graphics/renderer.hpp"
#include "graphics/texture.hpp"
#include <entt/entity/registry.hpp>

namespace destroid::drawing_system {

void update(entt::registry& registry, Renderer& renderer, const Texture& texture)
{
    const auto view = registry.view<Drawable, Transform>();
    view.each([&](const auto& drawable, const auto& transform) {
        renderer.draw(
            texture,
            drawable.region,
            transform.position,
            transform.rotation,
            1.0f);
    });
}

} // namespace destroid::drawing_system
