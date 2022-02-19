#include "Image.h"

#if _WIN32
#define STBIW_WINDOWS_UTF8
#endif

#include <stb_image.h>
#include <stb_image_write.h>

#include <assert.h>

namespace bamboo
{

constexpr auto TARGET_CHANNELS_COUNT = 4;

Image::Image()
  : m_width{ 0 }
  , m_height{ 0 }
  , m_data{ nullptr }
{
}

Image::Image(const std::filesystem::path& path)
  : Image()
{
    const auto buffer = convertPath(path);
    if (!std::filesystem::exists(path))
    {
        return;
    }
    m_data = stbi_load(buffer.get(), &m_width, &m_height, nullptr, TARGET_CHANNELS_COUNT);
    assert(m_data);
}

Image::Image(const Image& image) noexcept
  : m_width{ image.m_width }
  , m_height{ image.m_height }
  , m_data{ static_cast<ImageData>(malloc(size())) }
{
    assert(m_data);
    std::memcpy(m_data, image.m_data, image.size());
}

Image::Image(size_t width, size_t height)
  : m_width{ static_cast<int>(width) }
  , m_height{ static_cast<int>(height) }
  // it's very important to allocate memory with the same method that stbi uses for allocation
  , m_data{ static_cast<ImageData>(malloc(size())) }
{
    assert(m_data);
}

Image::Image(Image&& image) noexcept
  : m_width{ image.m_width }
  , m_height{ image.m_height }
  , m_data{ image.m_data }
{
    image.m_width = 0;
    image.m_height = 0;
    image.m_data = nullptr;
}

Image::~Image()
{
    destroy();
}

Image& Image::operator=(const Image& image) noexcept
{
    if (this != &image)
    {
        destroy();

        m_width = image.m_width;
        m_height = image.m_height;
        m_data = stbi_load_from_memory(
          image.m_data, image.size(), &m_width, &m_height, nullptr, TARGET_CHANNELS_COUNT);
    }

    return *this;
}

Image& Image::operator=(Image&& rhs) noexcept
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

int Image::getWidth() const
{
    return m_width;
}

int Image::getHeight() const
{
    return m_height;
}

ImageData Image::getData()
{
    return m_data;
}

const ImageData Image::getData() const
{
    return m_data;
}

size_t Image::size() const
{
    return m_width * m_height * TARGET_CHANNELS_COUNT;
}

void Image::save(const std::filesystem::path& path) const
{
    const auto buffer = convertPath(path);
    stbi_write_png(buffer.get(),
                   m_width,
                   m_height,
                   TARGET_CHANNELS_COUNT,
                   m_data,
                   m_width * TARGET_CHANNELS_COUNT);
}

ColorB Image::getPixel(size_t x, size_t y) const
{
    return const_cast<Image*>(this)->getPixel(x, y);
}

ColorB& Image::getPixel(size_t x, size_t y)
{
    unsigned char* p = &m_data[y * m_width * TARGET_CHANNELS_COUNT + x * TARGET_CHANNELS_COUNT];
    ColorB* c = reinterpret_cast<ColorB*>(p);
    return *c;
}

void Image::setPixel(size_t x, size_t y, const ColorB& color)
{
    auto& ref = getPixel(x, y);
    ref = color;
}

void Image::destroy()
{
    if (m_data)
    {
        stbi_image_free(m_data);
        m_data = nullptr;
    }
}

bool Image::isValid() const
{
    return m_data != nullptr;
}

Image::operator bool()
{
    return isValid();
}

std::shared_ptr<char[]> Image::convertPath(const std::filesystem::path& path) const
{
    constexpr auto BUFFER_SIZE = 4096;
    std::shared_ptr<char[]> buffer{ new char[BUFFER_SIZE] };
    stbiw_convert_wchar_to_utf8(buffer.get(), BUFFER_SIZE, path.c_str());
    return buffer;
}

}
