#pragma once

#include <glm/glm.hpp>

namespace bamboo
{
template<typename TColorChannel>
using Color = glm::vec<4, TColorChannel>;

// Color Byte
using ColorB = Color<uint8_t>;
static_assert(sizeof(ColorB) == 4 * sizeof(uint8_t));

// Color Float
using ColorF = Color<float>;

constexpr ColorF toColorF(const ColorB& color)
{
    return ColorF{ color } / 255.0f;
}

constexpr ColorB toColorB(const ColorF& color)
{
    return ColorB{ color * 255.0f };
}

template<typename TFrom, typename TTo>
constexpr Color<TTo> toColor(const Color<TFrom>& color)
{
    if constexpr (std::is_same_v<TFrom, TTo>)
    {
        return color;
    }
    if constexpr (std::is_same_v<TTo, uint8_t>)
    {
        return toColorB(color);
    }
    if constexpr (std::is_same_v<TTo, float>)
    {
        return toColorF(color);
    }
}

}
