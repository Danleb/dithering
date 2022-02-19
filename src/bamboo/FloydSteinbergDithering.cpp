#include "FloydSteinbergDithering.h"

#include "Color.h"
#include "Image.h"

namespace
{
bamboo::ColorF getClosestColor(const std::vector<bamboo::ColorF>& palette,
                               const bamboo::ColorF pixelValue)
{
    double minDist = std::numeric_limits<double>::max();
    bamboo::ColorF closest{};
    for (const auto color : palette)
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

} // namespace

namespace bamboo
{
FloydSteinbergDithering::FloydSteinbergDithering(DitheringConfiguration config)
  : m_config{ config }
{
}

void FloydSteinbergDithering::dither(Image& image)
{
    const auto palette = m_config.getPalette();

    for (size_t y = 0; y < image.getHeight(); ++y)
    {
        for (size_t x = 0; x < image.getWidth(); ++x)
        {
            auto& pixelRef = image.getPixel(x, y);
            const auto currentColor = toColorF(pixelRef);
            const auto closestColor = getClosestColor(palette, currentColor);
            pixelRef = toColorB(closestColor);

            const auto quantization_error = currentColor - closestColor;

            const auto rightNewDiff = quantization_error * 7.0f / 16.0f;
            const auto leftDownNewDiff = quantization_error * 3.0f / 16.0f;
            const auto downNewDiff = quantization_error * 5.0f / 16.0f;
            const auto rightDownNewDiff = quantization_error * 1.0f / 16.0f;

            if (x + 1 < image.getWidth())
            {
                auto& right = image.getPixel(x + 1, y);
                right = toColorB(toColorF(right) + rightNewDiff);
            }
            if (y + 1 < image.getHeight())
            {
                if (x > 0)
                {
                    auto& leftDown = image.getPixel(x - 1, y + 1);
                    leftDown = toColorB(toColorF(leftDown) + leftDownNewDiff);
                }

                auto& down = image.getPixel(x, y + 1);
                down = toColorB(toColorF(down) + downNewDiff);

                if (x + 1 < image.getWidth())
                {
                    auto& rightDown = image.getPixel(x + 1, y + 1);
                    rightDown = toColorB(toColorF(rightDown) + rightDownNewDiff);
                }
            }
        }
    }
}

}
