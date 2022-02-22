#include "ColorMatcher.h"

#include <bamboo/Image.h>

#include <gtest/gtest.h>

namespace
{

constexpr bamboo::ColorB DefaultColor{ 100, 123, 212 };
constexpr bamboo::ColorB TransparentBlack{ 0, 0, 0 };
constexpr bamboo::ColorB OpaqueWhite{ 255, 255, 255 };

void fillImage(bamboo::ImageB& image, const bamboo::ColorB& color = DefaultColor)
{
    for (size_t x = 0; x < image.getWidth(); ++x)
    {
        for (size_t y = 0; y < image.getHeight(); ++y)
        {
            image.setPixel(x, y, color);
        }
    }
}
} // namespace

namespace bamboo::test
{
const auto CatPath = "./pics/Cat.jpg";
const auto BuildingPath = "./pics/Cathedral.jpg";
const auto LightGrayPath = "./pics/LightGray_c3.jpg";

TEST(ImageTest, DefaultConstructor)
{
    ImageB image{};
    EXPECT_FALSE(static_cast<bool>(image));
    EXPECT_FALSE(image.isValid());
    EXPECT_EQ(image.getWidth(), 0);
    EXPECT_EQ(image.getHeight(), 0);
    EXPECT_EQ(image.getData(), nullptr);
    EXPECT_EQ(image.size(), 0);
}

TEST(ImageTest, PathConstructor1)
{
    ImageB image{ CatPath };
    EXPECT_EQ(image.getWidth(), 1770);
    EXPECT_EQ(image.getHeight(), 1180);
    ASSERT_NE(image.getData(), nullptr);
    EXPECT_EQ(image.size(), 1770 * 1180 * 3);
    EXPECT_TRUE(static_cast<bool>(image));
    EXPECT_TRUE(image.isValid());
}

TEST(ImageTest, PathConstructor2)
{
    ImageB image{ BuildingPath };
    EXPECT_EQ(image.getWidth(), 1336);
    EXPECT_EQ(image.getHeight(), 1779);
    ASSERT_NE(image.getData(), nullptr);
    EXPECT_EQ(image.size(), 1336 * 1779 * 3);
    EXPECT_TRUE(static_cast<bool>(image));
    EXPECT_TRUE(image.isValid());
}

TEST(ImageFloatTest, LightGray_PathConstructor)
{
    ImageF image{ LightGrayPath };
    EXPECT_EQ(image.getWidth(), 1377);
    EXPECT_EQ(image.getHeight(), 1127);
    ASSERT_NE(image.getData(), nullptr);
    EXPECT_EQ(image.size(), 1377 * 1127 * 3 * 4);
    EXPECT_TRUE(static_cast<bool>(image));
    EXPECT_TRUE(image.isValid());
    EXPECT_THAT(image.getPixel(0, 0), ColorsFEqual(ColorF{ 0.7647, 0.7647, 0.7647 }));
}

TEST(ImageTest, ImageFromMemory)
{
    const auto width = 1024;
    const auto height = 512;
    ImageB image{ width, height };
    fillImage(image);

    EXPECT_EQ(image.getWidth(), 1024);
    EXPECT_EQ(image.getHeight(), 512);
    EXPECT_NE(image.getData(), nullptr);
    EXPECT_EQ(image.size(), width * height * 3);
    const ColorB color = image.getPixel(0, 0);
    EXPECT_EQ(DefaultColor, color);

    image.setPixel(0, 0, { 255, 255, 255 });
    EXPECT_EQ(image.getPixel(0, 0), ColorB(255, 255, 255));
}

TEST(ImageTest, CopyConstructor)
{
    ImageB im1{ CatPath };
    ImageB im2{ im1 };

    EXPECT_NE(im1.getData(), im2.getData());
    EXPECT_EQ(im1.getWidth(), im2.getWidth());
    EXPECT_EQ(im1.getHeight(), im2.getHeight());
    EXPECT_EQ(im1.getPixel(10, 10), im2.getPixel(10, 10));
    EXPECT_EQ(im1.getPixel(20, 20), im2.getPixel(20, 20));
    EXPECT_EQ(im1.getPixel(30, 30), im2.getPixel(30, 30));

    im1.setPixel(30, 30, TransparentBlack);
    EXPECT_EQ(im1.getPixel(30, 30), TransparentBlack);
    EXPECT_NE(im1.getPixel(30, 30), im2.getPixel(30, 30));
}

}
