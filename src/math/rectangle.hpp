#pragma once

#include <glm/vec2.hpp>

namespace destroid {

struct Rectangle {
    constexpr Rectangle()
        : x(0.F)
        , y(0.F)
        , width(0.F)
        , height(0.F)
    {
    }

    constexpr Rectangle(glm::vec2 position, glm::vec2 size)
        : x(position.x)
        , y(position.y)
        , width(size.x)
        , height(size.y)
    {
    }

    constexpr Rectangle(float xPos, float yPos, float sizeWidth, float sizeHeight)
        : x(xPos)
        , y(yPos)
        , width(sizeWidth)
        , height(sizeHeight)
    {
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
        return { x + (width * 0.5F), y + (height * 0.5F) };
    }

    float x;
    float y;
    float width;
    float height;
};

} // namespace destroid
