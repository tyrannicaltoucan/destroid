#include "factory.hpp"
#include "entity/components/collider.hpp"
#include "entity/components/drag.hpp"
#include "entity/components/drawable.hpp"
#include "entity/components/lifetime.hpp"
#include "entity/components/momentum.hpp"
#include "entity/components/spawner.hpp"
#include "entity/components/tag.hpp"
#include "entity/components/thrust.hpp"
#include "entity/components/transform.hpp"
#include "entity/components/weapon.hpp"
#include "math/circle.hpp"
#include "math/random.hpp"
#include "math/rectangle.hpp"
#include <entt/entity/registry.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec2.hpp>

namespace destroid::entity {
namespace {

constexpr Rect shipSource = Rect(0, 0, 32, 32);
constexpr float shipLinearDrag = 0.15f;
constexpr float shipAngularDrag = 0.02f;
constexpr float shipLinearSpeed = 200.0f;
constexpr float shipAngularSpeed = shipLinearSpeed * 2.0f;
constexpr float shipWeaponCooldown = 0.3f;
constexpr Rect bulletSource = Rect(0, 32, 8, 8);
constexpr float bulletSpeed = shipLinearSpeed * 1.35f;
constexpr float bulletTimeActive = 0.7f;
constexpr float asteroidMinSpeed = 25.0f;
constexpr float asteroidMaxSpeed = 65.0f;
constexpr int asteroidSpawnCap = 5;

} // namespace

entt::entity createShip(entt::registry& registry)
{
    const auto e = registry.create();

    const auto viewport = registry.ctx<Rect>();
    const glm::vec2 position = viewport.center();
    const auto collider = Circle(position, shipSource.width / 3.5f);

    registry.emplace<ShipTag>(e);
    registry.emplace<Collider>(e, collider);
    registry.emplace<Drag>(e, shipLinearDrag, shipAngularDrag);
    registry.emplace<Drawable>(e, shipSource);
    registry.emplace<Momentum>(e);
    registry.emplace<Transform>(e, position);
    registry.emplace<Thrust>(e, shipLinearSpeed, shipAngularSpeed);
    registry.emplace<Weapon>(e, shipWeaponCooldown);

    return e;
}

entt::entity createAsteroid(entt::registry& registry, const glm::vec2& position)
{
    const auto e = registry.create();

    Rect source;
    const int asteroidTypes = random::between(1, 3);
    switch (asteroidTypes) {
    case 1:
        source = Rect(32, 0, 32, 32);
        break;
    case 2:
        source = Rect(64, 0, 32, 32);
        break;
    case 3:
        source = Rect(96, 0, 32, 32);
        break;
    }

    // Calculate two rotations: one for the velocity direction, and one for visual orientation.
    const float direction = glm::radians(random::between(0.0f, 359.0f));
    const float orientation = random::between(0.0f, 359.0f);

    const float speedX = random::between(asteroidMinSpeed, asteroidMaxSpeed);
    const float speedY = random::between(asteroidMinSpeed, asteroidMaxSpeed);
    const auto speed = glm::vec2(speedX, speedY);
    const auto impulse = glm::vec2(random::either(-1.0f, 1.0f), random::either(-1.0f, 1.0f));
    const auto velocity = glm::vec2(glm::cos(direction), -glm::sin(direction)) * speed * impulse;
    const auto collider = Circle(position, shipSource.width / 3.5f);

    registry.emplace<AsteroidTag>(e);
    registry.emplace<Collider>(e, collider);
    registry.emplace<Drawable>(e, source);
    registry.emplace<Momentum>(e, velocity);
    registry.emplace<Transform>(e, position, orientation);

    return e;
}

entt::entity createBullet(entt::registry& registry, const glm::vec2& position, float angle)
{
    const auto e = registry.create();

    const float cos = glm::cos(glm::radians(angle));
    const float sin = -glm::sin(glm::radians(angle));
    const auto offset = glm::vec2(position.x + sin, position.y + cos);
    const auto collider = Circle(offset, bulletSource.width / 2.5f);
    const auto velocity = glm::vec2(bulletSpeed * sin, bulletSpeed * cos);

    registry.emplace<BulletTag>(e);
    registry.emplace<Collider>(e, collider);
    registry.emplace<Drawable>(e, bulletSource);
    registry.emplace<Lifetime>(e, bulletTimeActive);
    registry.emplace<Momentum>(e, velocity);
    registry.emplace<Transform>(e, offset, angle);

    return e;
}

entt::entity createSpawner(entt::registry& registry)
{
    const auto e = registry.create();

    registry.emplace<SpawnerTag>(e);
    registry.emplace<Spawner>(e, asteroidSpawnCap);

    return e;
}

} // namespace destroid::entity
