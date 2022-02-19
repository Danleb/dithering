#include "Color.h"

namespace bamboo
{

ColorF toColorF(const ColorB& color)
{
    return ColorF{ color } / 255.0f;
}

ColorB toColorB(const ColorF& color)
{
    return ColorB{ color * 255.0f };
}
}
