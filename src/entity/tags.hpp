#pragma once

#include <entt/core/type_traits.hpp>
#include <entt/core/hashed_string.hpp>

namespace destroid {

namespace {

    using namespace entt::literals;

}

using PlayerTag = entt::tag<"player"_hs>;
using AsteroidTag = entt::tag<"asteroid"_hs>;
using BulletTag = entt::tag<"bullet"_hs>;
using DespawnTag = entt::tag<"despawnable"_hs>;

} // namespace destroid::entity_tag
