#include <bamboo/DitheringConfiguration.h>
#include <bamboo/FloydSteinbergDithering.h>
#include <bamboo/Image.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Invalid number of arguments.";
        exit(2);
    }

    // load dithering configuration
    const std::filesystem::path configPath{ argv[1] };
    if (!std::filesystem::exists(configPath))
    {
        std::cerr << "Dithering configuration file not found at: " << configPath << std::endl;
        exit(3);
    }
    const auto config = bamboo::DitheringConfiguration::loadConfiguration(configPath);
    if (!config)
    {
        std::cerr << "Failed to load dithering configuration at: " << configPath << std::endl;
        exit(4);
    }

    // load image
    const std::filesystem::path imagePath{ argv[2] };
    if (!std::filesystem::exists(imagePath))
    {
        std::cerr << "Failed to load image at: " << imagePath << std::endl;
        exit(5);
    }
    bamboo::ImageF image{ imagePath };

    // dither the image
    bamboo::FloydSteinbergDithering ditheringAlg{ *config };
    ditheringAlg.dither(image);

    // save the image
    auto newFilename = imagePath.stem();
    newFilename.concat("_dithered.jpg");
    auto outputPath = imagePath;
    outputPath.replace_filename(newFilename);
    image.save(outputPath);

    exit(0);
}
