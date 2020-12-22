#pragma once

#include <entt/entity/fwd.hpp>
#include <glm/fwd.hpp>

namespace destroid::entity_factory {

entt::entity createPlayer(entt::registry& reg);
entt::entity createAsteroid(entt::registry& reg, const glm::vec2& pos, const glm::vec2& vel);
entt::entity createProjectile(entt::registry& reg, const glm::vec2& pos, float rot);

} // namespace destroid::entity_factory
