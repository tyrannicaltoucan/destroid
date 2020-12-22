#include "factory.hpp"
#include "tags.hpp"
#include "component/body.hpp"
#include "component/collider.hpp"
#include "component/drawable.hpp"
#include "component/lifetime.hpp"
#include "component/ship.hpp"
#include "component/transform.hpp"
#include "component/weapon.hpp"
#include "graphics/texture.hpp"
#include <entt/entity/registry.hpp>
#include <glm/trigonometric.hpp>
#include <memory>

namespace destroid::entity_factory {

namespace {

    constexpr float REGION_SIZE = 32.F;
    constexpr float BOUNDING_SIZE = REGION_SIZE / 3.5F;

} // namespace

entt::entity createPlayer(entt::registry& reg)
{
    const auto entity = reg.create();
    const auto& texture = reg.ctx<std::shared_ptr<Texture>>();
    const auto& viewport = reg.ctx<Rectangle>();
    const Rectangle texRegion{0.F, 0.F, REGION_SIZE, REGION_SIZE};
    const glm::vec2 position = viewport.center() - texRegion.center();
    const float thrustSpeed = 10.F;
    const float rotationSpeed = thrustSpeed / 4.F;
    const float drag = thrustSpeed / 4.F;
    const float weaponCooldown = 0.3F;

    reg.emplace<PlayerTag>(entity);
    reg.emplace<Transform>(entity, position);
    reg.emplace<Body>(entity, drag);
    reg.emplace<Collider>(entity, Circle{position, BOUNDING_SIZE});
    reg.emplace<Drawable>(entity, texture, texRegion);
    reg.emplace<Ship>(entity, thrustSpeed, rotationSpeed);
    reg.emplace<Weapon>(entity, weaponCooldown);

    return entity;
}

entt::entity createAsteroid(entt::registry& reg, const glm::vec2& pos, const glm::vec2& vel)
{
    const auto entity = reg.create();
    const auto& texture = reg.ctx<std::shared_ptr<Texture>>();
    const Rectangle texRegion{REGION_SIZE, 0.F, REGION_SIZE, REGION_SIZE};
    const float drag = 0.F;

    reg.emplace<AsteroidTag>(entity);
    reg.emplace<Transform>(entity, pos);
    reg.emplace<Body>(entity, drag, vel);
    reg.emplace<Collider>(entity, Circle{pos, BOUNDING_SIZE});
    reg.emplace<Drawable>(entity, texture, texRegion);

    return entity;
}

entt::entity createProjectile(entt::registry& reg, const glm::vec2& pos, float rot)
{
    const float regionSize = 8.F;
    const auto entity = reg.create();
    const auto& texture = reg.ctx<std::shared_ptr<Texture>>();
    const Rectangle texRegion{REGION_SIZE * 2.F, 0.F, regionSize, regionSize};
    const float speed = 125.F;
    const float rotationRads = glm::radians(rot);

    glm::vec2 offset;
    offset.x = glm::sin(rotationRads) * (regionSize / 2.F);
    offset.y = glm::cos(rotationRads) * (regionSize / 2.F);

    glm::vec2 velocity;
    velocity.x = glm::sin(rotationRads) * speed;
    velocity.y = glm::cos(rotationRads) * speed;

    const glm::vec2 offsetPosition = offset + pos;
    const float drag = 0.F;

    reg.emplace<ProjectileTag>(entity);
    reg.emplace<Transform>(entity, offsetPosition, rot);
    reg.emplace<Body>(entity, drag, velocity);
    reg.emplace<Collider>(entity, Circle{offsetPosition, regionSize / 2.5F});
    reg.emplace<Lifetime>(entity, 2.F);
    reg.emplace<Drawable>(entity, texture, texRegion);

    return entity;
}
} // namespace destroid::entity_factory
