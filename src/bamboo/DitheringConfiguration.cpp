#include "DitheringConfiguration.h"

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include <fstream>
#include <iomanip>
#include <sstream>

namespace
{

const auto PaletteObjName = "palette";

inline float toFloat(uint32_t integerColorChannel)
{
    return static_cast<float>(integerColorChannel) / 255.0f;
}

// Expects 3-component color without alpha-channel
bamboo::ColorF toColor(uint32_t v)
{
    const uint32_t rv = (v >> 16) & 0xFF;
    const uint32_t gv = (v >> 8) & 0xFF;
    const uint32_t bv = v & 0xFF;

    const float r = toFloat(rv);
    const float g = toFloat(gv);
    const float b = toFloat(bv);
    return { r, g, b, 1.0f };
}

std::optional<rapidjson::Document> loadJsonDoc(const std::filesystem::path& path)
{
    std::ifstream ifs(path);
    if (!ifs)
    {
        return std::nullopt;
    }
    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document d;
    // todo check if loaded correctly
    d.ParseStream(isw);
    return d;
}

} // namespace

namespace bamboo
{
std::optional<DitheringConfiguration> DitheringConfiguration::loadConfiguration(
  const std::filesystem::path& path)
{
    const auto configJsonDoc = loadJsonDoc(path);
    if (!configJsonDoc)
    {
        return std::nullopt;
    }
    return DitheringConfiguration{ *configJsonDoc };
}

DitheringConfiguration::DitheringConfiguration(const rapidjson::Document& configuration)
{
    if (!configuration.HasMember(PaletteObjName))
    {
        return;
    }

    const auto colors = configuration[PaletteObjName].GetArray();
    m_palette.reserve(colors.Size());
    for (const auto& v : colors)
    {
        const auto colorCodeStr = v.GetString();
        const auto length = v.GetStringLength();
        const auto index = (length > 0 && colorCodeStr[0] == '#') ? colorCodeStr + 1 : colorCodeStr;

        uint32_t colorCode;
        std::stringstream ss;
        ss << std::hex << colorCodeStr;
        ss >> colorCode;
        const auto color = toColor(colorCode);
        m_palette.push_back(color);
    }
}

const std::vector<ColorF>& DitheringConfiguration::getPalette() const
{
    return m_palette;
}

}
