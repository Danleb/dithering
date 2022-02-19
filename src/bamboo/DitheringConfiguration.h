#pragma once

#include "Color.h"

#include <rapidjson/document.h>

#include <filesystem>
#include <optional>
#include <vector>

namespace bamboo
{
class DitheringConfiguration
{
public:
    static std::optional<DitheringConfiguration> loadConfiguration(
      const std::filesystem::path& path);

    DitheringConfiguration(const rapidjson::Document& configuration);

    const std::vector<ColorF>& getPalette() const;

private:
    std::vector<ColorF> m_palette;
};
}
