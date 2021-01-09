#include "weapon.hpp"
#include "entity/factory.hpp"
#include "component/transform.hpp"
#include "component/weapon.hpp"
#include <entt/entity/registry.hpp>

namespace destroid::weapon_system {

void update(entt::registry& registry, float delta)
{
    const auto view = registry.view<Weapon, Transform>();
    view.each([&](auto& weapon, const auto& transform) {
        if (weapon.canFire) {
            weapon.canFire = false;
            entity_factory::spawnBullet(registry, transform.position, transform.rotation);
            weapon.cooldown = weapon.fireRate;
        }

        weapon.cooldown -= delta;
    });
}

} // namespace destroid::weapon_system
