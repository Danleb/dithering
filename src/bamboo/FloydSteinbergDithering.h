#pragma once

#include "DitheringConfiguration.h"
#include "Image.h"

namespace bamboo
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
