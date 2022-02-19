#include "FloydSteinbergDithering.h"

#include "Color.h"
#include "Image.h"

namespace bamboo
{
FloydSteinbergDithering::FloydSteinbergDithering(DitheringConfiguration config)
  : m_image{ nullptr }
  , m_config{ config }
{
}

void FloydSteinbergDithering::dither(ImageF& image)
{
    m_image = &image;
    const size_t mostRightX = m_image->getWidth() - 1;
    const size_t mostDownY = m_image->getHeight() - 1;

    for (size_t y = 0; y < mostDownY; ++y)
    {
        const auto mostLeftPixelQuantError = processPixel(0, y);
        passQuantErrorRight(0, y, mostLeftPixelQuantError);
        passQuantErrorDown(0, y, mostLeftPixelQuantError);
        passQuantErrorRightDown(0, y, mostLeftPixelQuantError);

        for (size_t x = 1; x < mostRightX; ++x)
        {
            const auto quantizationError = processPixel(x, y);
            passQuantErrorRight(x, y, quantizationError);
            passQuantErrorLeftDown(x, y, quantizationError);
            passQuantErrorDown(x, y, quantizationError);
            passQuantErrorRightDown(x, y, quantizationError);
        }

        const auto mostRightPixelQuantError = processPixel(mostRightX, y);
        passQuantErrorLeftDown(mostRightX, y, mostRightPixelQuantError);
        passQuantErrorDown(mostRightX, y, mostRightPixelQuantError);
    }

    // process last (most down) line
    for (size_t x = 0; x < mostRightX; ++x)
    {
        const auto quantizationError = processPixel(x, mostDownY);
        passQuantErrorRight(x, mostDownY, quantizationError);
    }

    // process down right (last) pixel
    processPixel(mostRightX, mostDownY);
}

bamboo::ColorF FloydSteinbergDithering::getClosestColor(const bamboo::ColorF pixelValue) const
{
    double minDist = std::numeric_limits<double>::max();
    bamboo::ColorF closest{};
    for (const auto color : m_config.getPalette())
    {
        const auto dist = glm::length(pixelValue - color);
        if (dist < minDist)
        {
            minDist = dist;
            closest = color;
        }
    }
    return closest;
}

bamboo::ColorF FloydSteinbergDithering::processPixel(size_t x, size_t y)
{
    auto& currentColor = m_image->getPixel(x, y);
    const auto closestColor = getClosestColor(currentColor);
    const auto quantizationError = currentColor - closestColor;
    currentColor = closestColor;
    return quantizationError;
}

}
