#pragma once

#include <entt/core/type_traits.hpp>
#include <entt/core/hashed_string.hpp>

namespace destroid {

namespace {

    using namespace entt::literals;

} // namespace

using AsteroidTag = entt::tag<"asteroid"_hs>;
using BulletTag = entt::tag<"bullet"_hs>;
using DespawnTag = entt::tag<"despawn"_hs>;
using ShipTag = entt::tag<"ship"_hs>;
using SpawnerTag = entt::tag<"spawner"_hs>;

} // namespace destroid
