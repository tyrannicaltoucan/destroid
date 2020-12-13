#pragma once

#include <entt/entity/fwd.hpp>
#include <glm/fwd.hpp>

namespace destroid::entity_factory {

entt::entity createPlayer(entt::registry& reg);
entt::entity createAsteroid(entt::registry& reg, const glm::vec2& pos, const glm::vec2& vel);

} // namespace destroid::entity_factory
