#include "factory.hpp"
#include "tags.hpp"
#include "base/random.hpp"
#include "component/drag.hpp"
#include "component/collider.hpp"
#include "component/drawable.hpp"
#include "component/lifetime.hpp"
#include "component/momentum.hpp"
#include "component/spawner.hpp"
#include "component/transform.hpp"
#include "component/thrust.hpp"
#include "component/weapon.hpp"
#include <entt/entity/registry.hpp>
#include <glm/trigonometric.hpp>

namespace destroid::entity_factory {

namespace {

    constexpr float DEFAULT_REGION_SIZE = 32.F;
    constexpr float DEFAULT_BOUNDING_SIZE = DEFAULT_REGION_SIZE / 3.5F;
    constexpr float PLAYER_LINEAR_DRAG = 0.15F;
    constexpr float PLAYER_ANGULAR_DRAG = 0.02F;
    constexpr float PLAYER_WEAPON_COOLDOWN = 0.3F;
    constexpr float PLAYER_THRUST_SPEED = 200.F;
    constexpr float PLAYER_ROTATION_SPEED = PLAYER_THRUST_SPEED * 2.F;
    constexpr float BULLET_SPEED = PLAYER_THRUST_SPEED * 1.25F;
    constexpr float BULLET_LIFETIME = 0.75F;
    constexpr int ASTEROID_SPAWN_CAP = 5;
    constexpr float ASTEROID_MIN_SPEED = 25.F;
    constexpr float ASTEROID_MAX_SPEED = ASTEROID_MIN_SPEED * 2.F;

} // namespace

entt::entity spawnPlayer(entt::registry& registry)
{
    const auto entity = registry.create();
    const auto region = Rectangle(0.F, 0.F, DEFAULT_REGION_SIZE, DEFAULT_REGION_SIZE);
    const auto viewport = registry.ctx<Rectangle>();
    const glm::vec2 position = viewport.center();

    registry.emplace<PlayerTag>(entity);
    registry.emplace<Drag>(entity, PLAYER_LINEAR_DRAG, PLAYER_ANGULAR_DRAG);
    registry.emplace<Transform>(entity, position);
    registry.emplace<Momentum>(entity);
    registry.emplace<Collider>(entity, Circle{position, DEFAULT_BOUNDING_SIZE});
    registry.emplace<Drawable>(entity, region);
    registry.emplace<Thrust>(entity, PLAYER_THRUST_SPEED, PLAYER_ROTATION_SPEED);
    registry.emplace<Weapon>(entity, PLAYER_WEAPON_COOLDOWN);

    return entity;
}

entt::entity spawnAsteroid(
    entt::registry& registry,
    const glm::vec2& position,
    const glm::vec2& orientation,
    float angle)
{
    const auto entity = registry.create();
    const auto region = Rectangle{
        DEFAULT_REGION_SIZE,
        0.F,
        DEFAULT_REGION_SIZE,
        DEFAULT_REGION_SIZE};

    const auto velocity = glm::vec2{
        random::between(ASTEROID_MIN_SPEED, ASTEROID_MAX_SPEED) * orientation.x,
        random::between(ASTEROID_MIN_SPEED, ASTEROID_MAX_SPEED) * orientation.y,
    };

    registry.emplace<AsteroidTag>(entity);
    registry.emplace<Transform>(entity, position, angle);
    registry.emplace<Momentum>(entity, velocity);
    registry.emplace<Collider>(entity, Circle{position, DEFAULT_BOUNDING_SIZE});
    registry.emplace<Drawable>(entity, region);

    return entity;
}

entt::entity spawnBullet(entt::registry& registry, const glm::vec2& parentPosition, float angle)
{
    const float regionSize = 8.F;
    const auto entity = registry.create();
    const auto region = Rectangle(DEFAULT_REGION_SIZE * 2.F, 0.F, regionSize, regionSize);
    const float cos = glm::cos(glm::radians(angle));
    const float sin = glm::sin(glm::radians(angle));

    const glm::vec2 position{
        parentPosition.x + sin * (regionSize / 2.F),
        parentPosition.y + cos * (regionSize / 2.F)};

    const glm::vec2 velocity{
        sin * BULLET_SPEED,
        cos * BULLET_SPEED};

    registry.emplace<BulletTag>(entity);
    registry.emplace<Transform>(entity, position, angle);
    registry.emplace<Momentum>(entity, velocity);
    registry.emplace<Collider>(entity, Circle{position, regionSize / 2.5F});
    registry.emplace<Lifetime>(entity, BULLET_LIFETIME);
    registry.emplace<Drawable>(entity, region);

    return entity;
}

entt::entity createSpawner(entt::registry& registry)
{
    const auto entity = registry.create();
    registry.emplace<Spawner>(entity, ASTEROID_SPAWN_CAP);

    return entity;
}

} // namespace destroid::entity_factory
