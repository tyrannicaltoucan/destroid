#pragma once

#include <glm/vec2.hpp>

namespace destroid {

struct Rect {
    constexpr Rect() = default;

    constexpr Rect(float xPos, float yPos, float sizeWidth, float sizeHeight)
        : x(xPos)
        , y(yPos)
        , width(sizeWidth)
        , height(sizeHeight)
    {
    }

    constexpr Rect(const glm::vec2& position, const glm::vec2& size)
        : Rect(position.x, position.y, size.x, size.y)
    {
    }

    constexpr glm::vec2 position() const
    {
        return {x, y};
    }

    constexpr glm::vec2 size() const
    {
        return {width, height};
    }

    constexpr float left() const
    {
        return x;
    }

    constexpr float right() const
    {
        return x + width;
    }

    constexpr float top() const
    {
        return y;
    }

    constexpr float bottom() const
    {
        return y + height;
    }

    constexpr glm::vec2 center() const
    {
        return {x + (width * 0.5f), y + (height * 0.5f)};
    }

    float x = 0.0f;
    float y = 0.0f;
    float width = 0.0f;
    float height = 0.0f;
};

} // namespace destroid
