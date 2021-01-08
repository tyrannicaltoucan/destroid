#pragma once

#include <entt/core/type_traits.hpp>
#include <entt/core/hashed_string.hpp>

namespace destroid {

using PlayerTag = entt::tag<"player"_hs>;
using AsteroidTag = entt::tag<"asteroid"_hs>;
using BulletTag = entt::tag<"bullet"_hs>;

} // namespace destroid::entity_tag
