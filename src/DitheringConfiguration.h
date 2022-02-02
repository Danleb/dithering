#pragma once

#include <glm/glm.hpp>
#include <rapidjson/document.h>

#include <vector>

namespace effects
{
using Color = glm::vec4;

class DitheringConfiguration
{
public:
    DitheringConfiguration(const rapidjson::Document& configuration);

private:
    std::vector<Color> m_palette;
};
}
