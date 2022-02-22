#pragma once

#include "Color.h"

#include <glm/glm.hpp>
#include <stb_image.h>

#include <cstring>
#include <filesystem>
#include <memory>
#include <variant>

namespace bamboo
{

/// <summary>
/// Image class of generic color channel type, but only 1-byte and float implementations are
/// available.
/// </summary>
/// <typeparam name="TColorChannel">Type that is used for storing color channel value. Can be either
/// uint8_t or float.</typeparam>
template<typename TColorChannel>
class Image
{
    static constexpr auto TARGET_CHANNELS_COUNT = 3;

public:
    using ImageData = TColorChannel*;
    using ConstImageData =  const ImageData;
    using ColorT = Color<TColorChannel>;
    using ConstColorRef = const ColorT&;

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
    Image(size_t width, size_t height);

    /// <summary>
    /// This constructor allows to convert from image with one channel storage type.
    /// </summary>
    /// <typeparam name="TSourceChannelType"></typeparam>
    /// <param name="image"></param>
    template<typename TSourceChannelType>
    Image(const Image<TSourceChannelType>& image)
      : m_width{ image.getWidth() }
      , m_height{ image.getHeight() }
      , m_data{ nullptr }
    {
        m_data = static_cast<ImageData>(malloc(size()));
        assert(m_data);

        if constexpr (std::is_same_v<TColorChannel, TSourceChannelType>)
        {
            // same storage type, just copy memory
            std::memcpy(m_data, image.getData(), size());
        }
        else
        {
            // different storage types, need to perform conversion

            //// todo parallel?
            for (size_t i = 0; i < getPixelsCount(); ++i)
            {
                const Color<TSourceChannelType>& pixelFrom = image.getPixel(i);
                Color<TColorChannel>& pixelTo = getPixel(i);
                pixelTo = toColor<TSourceChannelType, TColorChannel>(pixelFrom);
            }
        }
    }

    Image(const Image& image);
    Image(Image&& image) noexcept;
    ~Image();

    Image& operator=(const Image& image);
    Image& operator=(Image&& image) noexcept;

    int getWidth() const;
    int getHeight() const;
    ImageData getData();
    ConstImageData getData() const;
    size_t size() const;

    void save(const std::filesystem::path& path) const;

    /// <summary>
    /// Get color at pixel with specified coordinates.
    /// Point (0; 0) is at top-left corner of image.
    /// </summary>
    ColorT getPixel(size_t x, size_t y) const;

    /// <summary>
    /// Get color at pixel with specified coordinates.
    /// Point (0; 0) is at top-left corner of image.
    /// </summary>
    ColorT& getPixel(size_t x, size_t y);

    size_t getPixelsCount() const;
    ColorT& getPixel(size_t i);
    const ColorT& getPixel(size_t i) const;

    /// <summary>
    /// Set color at pixel with specified coordinates.
    /// Point (0; 0) is at top-left corner of image.
    /// </summary>
    void setPixel(size_t x, size_t y, const ColorT& color);

    bool isValid() const;
    operator bool();

private:
    void destroy();
    const Image<uint8_t> getImageForSave() const;

    int m_width;
    int m_height;
    ImageData m_data;
};

// Image, where type color channel is unsigned integer of 1 byte size. RGBA - 4 bytes.
typedef Image<uint8_t> ImageB;

// Image, where type color channel is float of 4 byte size. RGBA - 16 bytes.
typedef Image<float> ImageF;

template<typename TColorChannel>
Image<TColorChannel>::ColorT& Image<TColorChannel>::getPixel(size_t i)
{
    TColorChannel* p = &m_data[i * TARGET_CHANNELS_COUNT];
    auto c = reinterpret_cast<Image<TColorChannel>::ColorT*>(p);
    return *c;
}

template<typename TColorChannel>
Image<TColorChannel>::ConstColorRef Image<TColorChannel>::getPixel(size_t i) const
{
    return const_cast<Image<TColorChannel>*>(this)->getPixel(i);
}

}
