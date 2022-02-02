#include "Image.h"

#if _WIN32
#define STBIW_WINDOWS_UTF8
#endif

#include <stb_image.h>
#include <stb_image_write.h>

#include <assert.h>

namespace effects
{
constexpr auto TARGET_CHANNELS_COUNT = 4;

Image::Image(const std::filesystem::path& path)
  : m_width{ 0 }
  , m_height{ 0 }
  , m_channels{ 0 }
  , m_data{ nullptr }
{
    const auto buffer = convertPath(path);
    if (!std::filesystem::exists(path))
    {
        return;
    }
    m_data = stbi_load(buffer.get(), &m_width, &m_height, &m_channels, TARGET_CHANNELS_COUNT);
}

Image::~Image()
{
    if (m_data)
    {
        stbi_image_free(m_data);
        m_data = nullptr;
    }
}

int Image::width() const
{
    return m_width;
}

int Image::height() const
{
    return m_height;
}

ImageData Image::data()
{
    return m_data;
}

ImageData Image::data() const
{
    return m_data;
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

std::shared_ptr<char[]> Image::convertPath(const std::filesystem::path& path) const
{
    constexpr auto BUFFER_SIZE = 4096;
    std::shared_ptr<char[]> buffer{ new char[BUFFER_SIZE] };
    stbiw_convert_wchar_to_utf8(buffer.get(), BUFFER_SIZE, path.c_str());
    return buffer;
}

}
