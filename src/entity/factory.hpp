#pragma once

#include <entt/entity/fwd.hpp>
#include <glm/fwd.hpp>

namespace destroid::entity_factory {

entt::entity spawnPlayer(entt::registry& registry);

entt::entity spawnAsteroid(
    entt::registry& registry,
    const glm::vec2& position,
    const glm::vec2& orientation);

entt::entity spawnBullet(
    entt::registry& registry,
    const glm::vec2& parentPosition,
    float angle);

entt::entity createSpawner(entt::registry& registry);

} // namespace destroid::entity_factory
