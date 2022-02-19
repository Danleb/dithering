#pragma once

#include "DitheringConfiguration.h"
#include "Image.h"

namespace bamboo
{
class FloydSteinbergDithering
{
public:
    FloydSteinbergDithering(DitheringConfiguration config);
    void dither(ImageF& image);

private:
    bamboo::ColorF getClosestColor(const bamboo::ColorF pixelValue) const;

    /// <summary>
    /// Replaces current pixel with closest color from palette and returns quantization error.
    /// </summary>
    bamboo::ColorF processPixel(size_t x, size_t y);

    inline void passQuantErrorRight(size_t x, size_t y, const ColorF& quantizationError);
    inline void passQuantErrorLeftDown(size_t x, size_t y, const ColorF& quantizationError);
    inline void passQuantErrorDown(size_t x, size_t y, const ColorF& quantizationError);
    inline void passQuantErrorRightDown(size_t x, size_t y, const ColorF& quantizationError);

    ImageF* m_image;
    DitheringConfiguration m_config;
};

inline void FloydSteinbergDithering::passQuantErrorRight(size_t x,
                                                         size_t y,
                                                         const ColorF& quantizationError)
{
    auto& right = m_image->getPixel(x + 1, y);
    const auto rightNewDiff = quantizationError * 7.0f / 16.0f;
    right += rightNewDiff;
}

inline void FloydSteinbergDithering::passQuantErrorLeftDown(size_t x,
                                                            size_t y,
                                                            const ColorF& quantizationError)
{
    auto& leftDown = m_image->getPixel(x - 1, y + 1);
    const auto leftDownNewDiff = quantizationError * 3.0f / 16.0f;
    leftDown += leftDownNewDiff;
}

inline void FloydSteinbergDithering::passQuantErrorDown(size_t x,
                                                        size_t y,
                                                        const ColorF& quantizationError)
{
    auto& down = m_image->getPixel(x, y + 1);
    const auto downNewDiff = quantizationError * 5.0f / 16.0f;
    down += downNewDiff;
}

inline void FloydSteinbergDithering::passQuantErrorRightDown(size_t x,
                                                             size_t y,
                                                             const ColorF& quantizationError)
{
    auto& rightDown = m_image->getPixel(x + 1, y + 1);
    const auto rightDownNewDiff = quantizationError * 1.0f / 16.0f;
    rightDown += rightDownNewDiff;
}

}
