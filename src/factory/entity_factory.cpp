#include "entity_factory.hpp"
#include "base/circle.hpp"
#include "base/random.hpp"
#include "base/rectangle.hpp"
#include "component/collider.hpp"
#include "component/drag.hpp"
#include "component/drawable.hpp"
#include "component/lifetime.hpp"
#include "component/momentum.hpp"
#include "component/spawner.hpp"
#include "component/transform.hpp"
#include "component/thrust.hpp"
#include "component/weapon.hpp"
#include <entt/entity/registry.hpp>
#include <entt/core/type_traits.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec2.hpp>

namespace destroid::entity_factory {

namespace {

    using namespace entt::literals;

    constexpr Rectangle SHIP_SOURCE = Rectangle(0, 0, 32, 32);
    constexpr float SHIP_LINEAR_DRAG = 0.15F;
    constexpr float SHIP_ANGULAR_DRAG = 0.02F;
    constexpr float SHIP_LINEAR_SPEED = 200.F;
    constexpr float SHIP_ANGULAR_SPEED = SHIP_LINEAR_SPEED * 2.F;
    constexpr float SHIP_WEAPON_COOLDOWN = 0.3F;
    constexpr Rectangle BULLET_SOURCE = Rectangle(0, 32, 8, 8);
    constexpr float BULLET_SPEED = SHIP_LINEAR_SPEED * 1.35F;
    constexpr float BULLET_TIME_ACTIVE = 0.7F;
    constexpr float ASTEROID_MIN_SPEED = 25.F;
    constexpr float ASTEROID_MAX_SPEED = 65.F;
    constexpr int ASTEROID_SPAWNER_CAP = 5;

} // namespace

entt::entity createShip(entt::registry& registry)
{
    const auto e = registry.create();

    const auto viewport = registry.ctx<Rectangle>();
    const glm::vec2 position = viewport.center();
    const auto collider = Circle(position, SHIP_SOURCE.width / 3.5F);

    registry.emplace<entt::tag<"ship"_hs>>(e);
    registry.emplace<Collider>(e, collider);
    registry.emplace<Drag>(e, SHIP_LINEAR_DRAG, SHIP_ANGULAR_DRAG);
    registry.emplace<Drawable>(e, SHIP_SOURCE);
    registry.emplace<Momentum>(e);
    registry.emplace<Transform>(e, position);
    registry.emplace<Thrust>(e, SHIP_LINEAR_SPEED, SHIP_ANGULAR_SPEED);
    registry.emplace<Weapon>(e, SHIP_WEAPON_COOLDOWN);

    return e;
}

entt::entity createAsteroid(entt::registry& registry, const glm::vec2& position)
{
    const auto e = registry.create();

    Rectangle source;
    const int asteroidTypes = random::between(1, 3);
    switch (asteroidTypes) {
    case 1:
        source = Rectangle(32, 0, 32, 32);
        break;
    case 2:
        source = Rectangle(64, 0, 32, 32);
        break;
    case 3:
        source = Rectangle(96, 0, 32, 32);
        break;
    }

    // Calculate two rotations: one for the velocity direction, and one for visual orientation.
    const float direction = glm::radians(random::between(0.F, 359.F));
    const float orientation = random::between(0.F, 359.F);

    const float speedX = random::between(ASTEROID_MIN_SPEED, ASTEROID_MAX_SPEED);
    const float speedY = random::between(ASTEROID_MIN_SPEED, ASTEROID_MAX_SPEED);
    const auto speed = glm::vec2(speedX, speedY);
    const auto impulse = glm::vec2(random::either(-1.F, 1.F), random::either(-1.F, 1.F));
    const auto velocity = glm::vec2(glm::cos(direction), -glm::sin(direction)) * speed * impulse;
    const auto collider = Circle(position, SHIP_SOURCE.width / 3.5F);

    registry.emplace<entt::tag<"asteroid"_hs>>(e);
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
    const auto collider = Circle(offset, BULLET_SOURCE.width / 2.5F);
    const auto velocity = glm::vec2(BULLET_SPEED * sin, BULLET_SPEED * cos);

    registry.emplace<entt::tag<"bullet"_hs>>(e);
    registry.emplace<Collider>(e, collider);
    registry.emplace<Drawable>(e, BULLET_SOURCE);
    registry.emplace<Lifetime>(e, BULLET_TIME_ACTIVE);
    registry.emplace<Momentum>(e, velocity);
    registry.emplace<Transform>(e, offset, angle);

    return e;
}

entt::entity createSpawner(entt::registry& registry)
{
    const auto e = registry.create();

    registry.emplace<entt::tag<"spawner"_hs>>(e);
    registry.emplace<Spawner>(e, ASTEROID_SPAWNER_CAP);

    return e;
}

} // namespace destroid::entity_factory
