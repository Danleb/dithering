#pragma once

#include "Color.h"

#include <glm/glm.hpp>
#include <stb_image.h>

#include <filesystem>
#include <memory>
#include <variant>

namespace bamboo
{
using ChannelVal = unsigned char;
using ImageData = unsigned char*;

// template<typename TChannelType>
// class Image;

class Image
{
public:
    Image();

    /// <summary>
    /// Loads image data from filesystem using stbi library.
    /// </summary>
    /// <param name="path"></param>
    Image(const std::filesystem::path& path);

    /// <summary>
    /// Constructs raw image data in memory.
    /// </summary>
    /// <param name="width">Image width</param>
    /// <param name="height">Image height</param>
    /// <param name="copy">Whether image data should be copied or no.</param>
    Image(size_t width, size_t height);

    Image(const Image& image) noexcept;
    Image(Image&& image) noexcept;
    ~Image();

    Image& operator=(const Image& image) noexcept;
    Image& operator=(Image&& image) noexcept;

    int getWidth() const;
    int getHeight() const;
    ImageData getData();
    const ImageData getData() const;
    size_t size() const;

    void save(const std::filesystem::path& path) const;

    ColorB getPixel(size_t x, size_t y) const;
    ColorB& getPixel(size_t x, size_t y);

    void setPixel(size_t x, size_t y, const ColorB& color);

    bool isValid() const;
    operator bool();

private:
    void destroy();

    std::shared_ptr<char[]> convertPath(const std::filesystem::path& path) const;

    int m_width;
    int m_height;
    ImageData m_data;
};

// typedef Image<unsigned char> ImageB;
// typedef Image<float> ImageF;

}
