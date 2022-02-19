#include <bamboo/DitheringConfiguration.h>
#include <bamboo/FloydSteinbergDithering.h>

#include <benchmark/benchmark.h>

void BM_FloydSteinbergDithering(benchmark::State& state,
                                const std::string& imagePath,
                                const std::string& configPath)
{
    const auto config = bamboo::DitheringConfiguration::loadConfiguration(configPath);
    const bamboo::Image originalImage{ imagePath };

    for (auto _ : state)
    {
        bamboo::Image imageToDither{ originalImage };
        bamboo::FloydSteinbergDithering alg(*config);
        alg.dither(imageToDither);
        benchmark::DoNotOptimize(imageToDither.getPixel(0, 0));
    }
}

void BM_FloydSteinbergDithering_BlackWhiteCat(benchmark::State& state)
{
    const auto imagePath = "../../../../pics/Cat.jpg";
    const auto configPath = "../../../../configs/BlackWhite.json";
    BM_FloydSteinbergDithering(state, imagePath, configPath);
}

void BM_FloydSteinbergDithering_BlackWhiteParis(benchmark::State& state)
{
    const auto imagePath = "../../../../pics/Paris.jpg";
    const auto configPath = "../../../../configs/BlackWhite.json";
    BM_FloydSteinbergDithering(state, imagePath, configPath);
}

void BM_FloydSteinbergDithering_RGB_CityView(benchmark::State& state)
{
    const auto imagePath = "../../../../pics/CityView.jpg";
    const auto configPath = "../../../../configs/RGB_Palette.json";
    BM_FloydSteinbergDithering(state, imagePath, configPath);
}

BENCHMARK(BM_FloydSteinbergDithering_BlackWhiteCat);
BENCHMARK(BM_FloydSteinbergDithering_BlackWhiteParis);
BENCHMARK(BM_FloydSteinbergDithering_RGB_CityView);
