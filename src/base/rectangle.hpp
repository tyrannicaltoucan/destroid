#pragma once

#include <glm/vec2.hpp>

namespace destroid {

struct Rectangle {
    constexpr Rectangle() = default;

    constexpr Rectangle(float xPos, float yPos, float sizeWidth, float sizeHeight)
        : x(xPos)
        , y(yPos)
        , width(sizeWidth)
        , height(sizeHeight)
    {
    }

    constexpr Rectangle(const glm::vec2& position, const glm::vec2& size)
        : Rectangle(position.x, position.y, size.x, size.y)
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
        return {x + (width * 0.5F), y + (height * 0.5F)};
    }

    float x = 0.F;
    float y = 0.F;
    float width = 0.F;
    float height = 0.F;
};

} // namespace destroid
