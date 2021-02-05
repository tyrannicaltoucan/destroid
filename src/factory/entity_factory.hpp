#pragma once

#include <entt/entity/fwd.hpp>
#include <glm/fwd.hpp>

namespace destroid::entity_factory {

entt::entity createShip(entt::registry& registry);
entt::entity createAsteroid(entt::registry& registry, const glm::vec2& position);
entt::entity createBullet(entt::registry& registry, const glm::vec2& shooterPosition, float angle);
entt::entity createSpawner(entt::registry& registry);

} // namespace destroid::entity_factory
