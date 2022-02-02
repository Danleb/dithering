#pragma once

#include "DitheringConfiguration.h"
#include "Image.h"

namespace effects
{
class FloydSteinbergDithering
{
public:
    FloydSteinbergDithering(DitheringConfiguration config);

    void dither(Image& image);

private:
    DitheringConfiguration m_config;
};
}
