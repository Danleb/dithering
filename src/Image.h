#pragma once

#include <filesystem>
#include <memory>

namespace effects
{
using ImageData = unsigned char*;

class Image
{
public:
    Image(const std::filesystem::path& path);
    ~Image();

    int width() const;
    int height() const;
    ImageData data();
    ImageData data() const;

    void save(const std::filesystem::path& path) const;

private:
    std::shared_ptr<char[]> convertPath(const std::filesystem::path& path) const;

    int m_width;
    int m_height;
    int m_channels;
    ImageData m_data;
};
}
