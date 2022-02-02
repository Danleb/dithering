#include "DitheringConfiguration.h"

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
effects::Color toColor(uint32_t v)
{
    const uint32_t rv = (v >> 16) & 0xFF;
    const uint32_t gv = (v >> 8) & 0xFF;
    const uint32_t bv = v & 0xFF;

    const float r = toFloat(rv);
    const float g = toFloat(gv);
    const float b = toFloat(bv);
    return { r, g, b, 1.0f };
}

} // namespace

namespace effects
{
DitheringConfiguration::DitheringConfiguration(const rapidjson::Document& configuration)
{
    if (configuration.HasMember(PaletteObjName))
    {
        const auto colors = configuration[PaletteObjName].GetArray();
        m_palette.reserve(colors.Size());
        for (const auto& v : colors)
        {
            const auto colorCodeStr = v.GetString();
            const auto length = v.GetStringLength();
            const auto index =
              (length > 0 && colorCodeStr[0] == '#') ? colorCodeStr + 1 : colorCodeStr;

            uint32_t colorCode;
            std::stringstream ss;
            ss << std::hex << colorCodeStr;
            ss >> colorCode;
            const auto color = toColor(colorCode);
            m_palette.push_back(color);
        }
    }
}
}
