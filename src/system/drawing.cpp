#include "drawing.hpp"
#include "component/drawable.hpp"
#include "component/transform.hpp"

namespace destroid::drawing_system {

void update(entt::registry& registry, Renderer& renderer)
{
    const auto view = registry.view<Drawable, Transform>();
    view.each([&](const Drawable& drawable, const Transform& transform) {
        renderer.draw(*drawable.texture, drawable.region, transform.position);
    });
}

} // namespace destroid::drawing_system
