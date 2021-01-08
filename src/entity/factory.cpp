#include "factory.hpp"
#include "tags.hpp"
#include "component/drag.hpp"
#include "component/collider.hpp"
#include "component/drawable.hpp"
#include "component/lifetime.hpp"
#include "component/momentum.hpp"
#include "component/transform.hpp"
#include "component/thrust.hpp"
#include "component/weapon.hpp"
#include "graphics/texture.hpp"
#include <entt/entity/registry.hpp>
#include <glm/trigonometric.hpp>

namespace destroid::entity_factory {

namespace {

    constexpr float REGION_SIZE = 32.F;
    constexpr float BOUNDING_SIZE = REGION_SIZE / 3.5F;

} // namespace

entt::entity createPlayer(entt::registry& reg)
{
    const auto entity = reg.create();
    const auto& viewport = reg.ctx<Rectangle>();
    const Rectangle texRegion{0.F, 0.F, REGION_SIZE, REGION_SIZE};
    const glm::vec2 position = viewport.center();
    const float thrustSpeed = 200.F;
    const float rotationSpeed = thrustSpeed * 1.7F;
    const float linearDrag = 0.25F;
    const float angularDrag = 0.05F;
    const float weaponCooldown = 0.3F;

    reg.emplace<PlayerTag>(entity);
    reg.emplace<Drag>(entity, linearDrag, angularDrag);
    reg.emplace<Transform>(entity, position);
    reg.emplace<Momentum>(entity);
    reg.emplace<Collider>(entity, Circle{position, BOUNDING_SIZE});
    reg.emplace<Drawable>(entity, texRegion);
    reg.emplace<Thrust>(entity, thrustSpeed, rotationSpeed);
    reg.emplace<Weapon>(entity, weaponCooldown);

    return entity;
}

entt::entity createAsteroid(entt::registry& reg, const glm::vec2& pos, const glm::vec2& vel)
{
    const auto entity = reg.create();
    const Rectangle texRegion{REGION_SIZE, 0.F, REGION_SIZE, REGION_SIZE};

    reg.emplace<AsteroidTag>(entity);
    reg.emplace<Transform>(entity, pos);
    reg.emplace<Momentum>(entity, vel);
    reg.emplace<Collider>(entity, Circle{pos, BOUNDING_SIZE});
    reg.emplace<Drawable>(entity, texRegion);

    return entity;
}

entt::entity createProjectile(entt::registry& reg, const glm::vec2& pos, float rot)
{
    const float regionSize = 8.F;
    const auto entity = reg.create();
    const Rectangle texRegion{REGION_SIZE * 2.F, 0.F, regionSize, regionSize};
    const float speed = 250.F;
    const float rotationRads = glm::radians(rot);

    glm::vec2 offset;
    offset.x = glm::sin(rotationRads) * (regionSize / 2.F);
    offset.y = glm::cos(rotationRads) * (regionSize / 2.F);

    glm::vec2 velocity;
    velocity.x = glm::sin(rotationRads) * speed;
    velocity.y = glm::cos(rotationRads) * speed;

    const glm::vec2 offsetPosition = offset + pos;

    reg.emplace<BulletTag>(entity);
    reg.emplace<Transform>(entity, offsetPosition, rot);
    reg.emplace<Momentum>(entity, velocity);
    reg.emplace<Collider>(entity, Circle{offsetPosition, regionSize / 2.5F});
    reg.emplace<Lifetime>(entity, 0.75F);
    reg.emplace<Drawable>(entity, texRegion);

    return entity;
}

} // namespace destroid::entity_factory
