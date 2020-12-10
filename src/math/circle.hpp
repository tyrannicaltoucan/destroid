#pragma once

#include <glm/vec2.hpp>

namespace destroid {

struct Circle {
    constexpr Circle()
        : x(0.F)
        , y(0.F)
        , radius(0.F)
    {
    }

    constexpr Circle(const glm::vec2& origin, float sizeRadius)
        : x(origin.x)
        , y(origin.y)
        , radius(sizeRadius)
    {
    }

    constexpr Circle(float originX, float originY, float sizeRadius)
        : x(originX)
        , y(originY)
        , radius(sizeRadius)
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

    float x;
    float y;
    float radius;
};

} // namespace destroid
