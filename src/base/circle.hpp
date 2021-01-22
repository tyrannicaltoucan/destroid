#pragma once

#include <glm/vec2.hpp>

namespace destroid {

struct Circle {
    constexpr Circle() = default;

    constexpr Circle(float originX, float originY, float sizeRadius)
        : x(originX)
        , y(originY)
        , radius(sizeRadius)
    {
    }

    constexpr Circle(const glm::vec2& origin, float sizeRadius)
        : Circle(origin.x, origin.y, sizeRadius)
    {
    }

    constexpr bool contains(const glm::vec2& point) const
    {
        const float dx = x - point.x;
        const float dy = y - point.y;
        return dx * dx + dy * dy <= radius * radius;
    }

    constexpr bool intersects(const Circle& other) const
    {
        const float dx = x - other.x;
        const float dy = y - other.y;
        const float distance = dx * dx + dy * dy;
        const float radiusSum = radius + other.radius;
        return distance < radiusSum * radiusSum;
    }

    float x = 0.F;
    float y = 0.F;
    float radius = 0.F;
};

} // namespace destroid
