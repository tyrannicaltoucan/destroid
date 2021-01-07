#include "firing.hpp"
#include "entity/factory.hpp"
#include "component/transform.hpp"
#include "component/weapon.hpp"

namespace destroid::firing_system {

void update(entt::registry& registry, float delta)
{
    const auto view = registry.view<Weapon, Transform>();

    for (const auto& entity : view) {
        const auto [transform, weapon] = view.get<Transform, Weapon>(entity);

        if (weapon.canFire) {
            weapon.canFire = false;
            entity_factory::createProjectile(registry, transform.position, transform.rotation);
            weapon.cooldown = weapon.fireRate;
        }

        weapon.cooldown -= delta;
    }
}

} // namespace destroid::firing_system
