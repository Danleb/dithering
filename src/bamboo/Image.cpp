#include "Image.h"

#if _WIN32
#define STBIW_WINDOWS_UTF8
#endif

#include <stb_image.h>
#include <stb_image_write.h>

#include <assert.h>

namespace
{

std::shared_ptr<char[]> convertPath(const std::filesystem::path& path)
{
    constexpr auto BUFFER_SIZE = 4096;
    std::shared_ptr<char[]> buffer{ new char[BUFFER_SIZE] };
    stbiw_convert_wchar_to_utf8(buffer.get(), BUFFER_SIZE, path.c_str());
    return buffer;
}

} // namespace

namespace bamboo
{

// Instantiate template Image class only for 2 types, since only two will be supported.
template class Image<uint8_t>;
template class Image<float>;

template<typename TColorChannel>
Image<TColorChannel>::Image()
  : m_width{ 0 }
  , m_height{ 0 }
  , m_data{ nullptr }
{
}

template<typename TColorChannel>
Image<TColorChannel>::Image(const std::filesystem::path& path)
  : Image()
{
    const auto buffer = convertPath(path);
    if (!std::filesystem::exists(path))
    {
        return;
    }

    if constexpr (std::is_same_v<TColorChannel, uint8_t>)
    {
        m_data = stbi_load(buffer.get(), &m_width, &m_height, nullptr, TARGET_CHANNELS_COUNT);
    }
    else
    {
        // we want to load image to float buffer, but without gamma correction
        stbi_ldr_to_hdr_gamma(1.0f);
        m_data = stbi_loadf(buffer.get(), &m_width, &m_height, nullptr, TARGET_CHANNELS_COUNT);
        stbi_ldr_to_hdr_gamma(2.2f);
    }

    assert(m_data);
}

template<typename TColorChannel>
Image<TColorChannel>::Image(size_t width, size_t height)
  : m_width{ static_cast<int>(width) }
  , m_height{ static_cast<int>(height) }
  // it's very important to allocate memory with the same method that stbi uses for allocation
  , m_data{ static_cast<ImageData>(malloc(size())) }
{
    assert(m_data);
}

template<typename TColorChannel>
Image<TColorChannel>::Image(const Image& image)
  : m_width{ image.m_width }
  , m_height{ image.m_height }
  , m_data{ static_cast<ImageData>(malloc(size())) }
{
    assert(m_data);
    std::memcpy(m_data, image.m_data, size());
}

template<typename TColorChannel>
Image<TColorChannel>::Image(Image<TColorChannel>&& image) noexcept
  : m_width{ image.m_width }
  , m_height{ image.m_height }
  , m_data{ image.m_data }
{
    image.m_width = 0;
    image.m_height = 0;
    image.m_data = nullptr;
}

template<typename TColorChannel>
Image<TColorChannel>::~Image()
{
    destroy();
}

template<typename TColorChannel>
Image<TColorChannel>& Image<TColorChannel>::operator=(const Image<TColorChannel>& image)
{
    if (this != &image)
    {
        destroy();

        m_width = image.m_width;
        m_height = image.m_height;
        m_data = static_cast<ImageData>(malloc(size()));
        assert(m_data);
        std::memcpy(m_data, image.m_data, size());
    }

    return *this;
}

template<typename TColorChannel>
Image<TColorChannel>& Image<TColorChannel>::operator=(Image&& rhs) noexcept
{
    if (this == &rhs)
    {
        m_width = rhs.m_width;
        m_height = rhs.m_height;
        m_data = rhs.m_data;

        rhs.m_width = 0;
        rhs.m_height = 0;
        rhs.m_data = nullptr;
    }

    return *this;
}

template<typename TColorChannel>
int Image<TColorChannel>::getWidth() const
{
    return m_width;
}

template<typename TColorChannel>
int Image<TColorChannel>::getHeight() const
{
    return m_height;
}

template<typename TColorChannel>
Image<TColorChannel>::ImageData Image<TColorChannel>::getData()
{
    return m_data;
}

template<typename TColorChannel>
Image<TColorChannel>::ConstImageData Image<TColorChannel>::getData() const
{
    return m_data;
}

template<typename TColorChannel>
size_t Image<TColorChannel>::size() const
{
    return m_width * m_height * TARGET_CHANNELS_COUNT * sizeof(TColorChannel);
}

template<typename TColorChannel>
void Image<TColorChannel>::save(const std::filesystem::path& path) const
{
    const auto buffer = convertPath(path);
    const auto& tempImage = getImageForSave();

    const auto extension = path.extension();
    if (extension == ".png")
    {
        assert(stbi_write_png(buffer.get(),
                              m_width,
                              m_height,
                              TARGET_CHANNELS_COUNT,
                              tempImage.getData(),
                              m_width * TARGET_CHANNELS_COUNT));
    }
    if (extension == ".jpg")
    {
        // from 1 to 100
        const auto quality = 100;
        stbi_write_jpg(
          buffer.get(), m_width, m_height, TARGET_CHANNELS_COUNT, tempImage.getData(), quality);
    }
}

template<typename TColorChannel>
Image<TColorChannel>::ColorT Image<TColorChannel>::getPixel(size_t x, size_t y) const
{
    return const_cast<Image*>(this)->getPixel(x, y);
}

template<typename TColorChannel>
Image<TColorChannel>::ColorT& Image<TColorChannel>::getPixel(size_t x, size_t y)
{
    TColorChannel* p = &m_data[y * m_width * TARGET_CHANNELS_COUNT + x * TARGET_CHANNELS_COUNT];
    auto c = reinterpret_cast<Image<TColorChannel>::ColorT*>(p);
    return *c;
}

template<typename TColorChannel>
void Image<TColorChannel>::setPixel(size_t x, size_t y, const Image<TColorChannel>::ColorT& color)
{
    auto& ref = getPixel(x, y);
    ref = color;
}

template<typename TColorChannel>
bool Image<TColorChannel>::isValid() const
{
    return m_data != nullptr;
}

template<typename TColorChannel>
Image<TColorChannel>::operator bool()
{
    return isValid();
}

template<typename TColorChannel>
size_t Image<TColorChannel>::getPixelsCount() const
{
    return m_width * m_height;
}

template<typename TColorChannel>
const Image<uint8_t> Image<TColorChannel>::getImageForSave() const
{
    if constexpr (std::is_same_v<TColorChannel, uint8_t>)
    {
        return *this;
    }
    if constexpr (std::is_same_v<TColorChannel, float>)
    {
        return ImageB(*this);
    }
}

template<typename TColorChannel>
void Image<TColorChannel>::destroy()
{
    if (m_data)
    {
        stbi_image_free(m_data);
        m_data = nullptr;
    }
}

}
