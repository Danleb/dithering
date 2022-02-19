#pragma once

#include <glm/glm.hpp>

namespace bamboo
{
// Color Byte
using ColorB = glm::vec<4, unsigned char>;
static_assert(sizeof(ColorB) == 4 * sizeof(unsigned char));

// Color Float
using ColorF = glm::vec4;

ColorF toColorF(const ColorB& color);
ColorB toColorB(const ColorF& color);

}
