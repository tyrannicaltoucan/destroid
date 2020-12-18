#include "factory.hpp"
#include "tags.hpp"
#include "component/collider.hpp"
#include "component/drawable.hpp"
#include "component/player.hpp"
#include "component/transform.hpp"
#include "component/velocity.hpp"
#include "graphics/texture.hpp"
#include <entt/entity/registry.hpp>
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
    const float damping = 0.95F;

    reg.emplace<PlayerTag>(entity);
    reg.emplace<Player>(entity, thrustSpeed, rotationSpeed, damping);
    reg.emplace<Transform>(entity, position);
    reg.emplace<Velocity>(entity);
    reg.emplace<Collider>(entity, Circle{position, BOUNDING_SIZE});
    reg.emplace<Drawable>(entity, texture, texRegion);

    return entity;
}

entt::entity createAsteroid(entt::registry& reg, const glm::vec2& pos, const glm::vec2& vel)
{
    const auto entity = reg.create();
    const auto& texture = reg.ctx<std::shared_ptr<Texture>>();
    const Rectangle texRegion{REGION_SIZE, 0.F, REGION_SIZE, REGION_SIZE};

    reg.emplace<AsteroidTag>(entity);
    reg.emplace<Transform>(entity, pos);
    reg.emplace<Velocity>(entity, vel);
    reg.emplace<Collider>(entity, Circle{pos, BOUNDING_SIZE});
    reg.emplace<Drawable>(entity, texture, texRegion);

    return entity;
}

} // namespace destroid::entity_factory
