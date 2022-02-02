#include "DitheringConfiguration.h"
#include "FloydSteinbergDithering.h"
#include "Image.h"

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>

std::optional<rapidjson::Document> loadConfiguration(const std::filesystem::path& path)
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

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Invalid number of arguments.";
        exit(2);
    }

    // load dithering configuration
    const std::filesystem::path configPath{ argv[1] };
    const auto configJson = loadConfiguration(configPath);
    if (!configJson)
    {
        std::cerr << "Failed to load dithering configuration.";
        exit(3);
    }
    const effects::DitheringConfiguration config{ *configJson };

    // load image
    const std::filesystem::path imagePath{ argv[2] };
    effects::Image image{ imagePath };

    // dither the image
    effects::FloydSteinbergDithering ditheringAlg{ config };
    ditheringAlg.dither(image);

    // save the image
    auto newFilename = imagePath.stem();
    newFilename.concat("_dithered.png");
    auto outputPath = imagePath;
    outputPath.replace_filename(newFilename);
    image.save(outputPath);

    return EXIT_SUCCESS;
}
