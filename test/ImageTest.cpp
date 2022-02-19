#include <bamboo/Image.h>

#include <gtest/gtest.h>

namespace
{

constexpr bamboo::ColorB DefaultColor{ 100, 123, 212, 255 };
constexpr bamboo::ColorB TransparentBlack{ 0, 0, 0, 0 };
constexpr bamboo::ColorB OpaqueWhite{ 255, 255, 255, 255 };

void fillImage(bamboo::Image& image, const bamboo::ColorB& color = DefaultColor)
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
const auto CatPath = "../../../../pics/Cat.jpg";
const auto BuildingPath = "../../../../pics/Cathedral.jpg";

TEST(ImageTest, DefaultConstructor)
{
    Image image{};
    EXPECT_FALSE(static_cast<bool>(image));
    EXPECT_FALSE(image.isValid());
    EXPECT_EQ(image.getWidth(), 0);
    EXPECT_EQ(image.getHeight(), 0);
    EXPECT_EQ(image.getData(), nullptr);
    EXPECT_EQ(image.size(), 0);
}

TEST(ImageTest, PathConstructor1)
{
    Image image{ CatPath };
    EXPECT_EQ(image.getWidth(), 1770);
    EXPECT_EQ(image.getHeight(), 1180);
    EXPECT_NE(image.getData(), nullptr);
    EXPECT_EQ(image.size(), 1770 * 1180 * 4);
    EXPECT_TRUE(static_cast<bool>(image));
    EXPECT_TRUE(image.isValid());
}

TEST(ImageTest, PathConstructor2)
{
    Image image{ BuildingPath };
    EXPECT_EQ(image.getWidth(), 1336);
    EXPECT_EQ(image.getHeight(), 1779);
    EXPECT_NE(image.getData(), nullptr);
    EXPECT_EQ(image.size(), 1336 * 1779 * 4);
    EXPECT_TRUE(static_cast<bool>(image));
    EXPECT_TRUE(image.isValid());
}

TEST(ImageTest, ImageFromMemory)
{
    const auto width = 1024;
    const auto height = 512;
    Image image{ width, height };
    fillImage(image);

    EXPECT_EQ(image.getWidth(), 1024);
    EXPECT_EQ(image.getHeight(), 512);
    EXPECT_NE(image.getData(), nullptr);
    EXPECT_EQ(image.size(), width * height * 4);
    const ColorB color = image.getPixel(0, 0);
    EXPECT_EQ(DefaultColor, color);

    image.setPixel(0, 0, { 255, 255, 255, 255 });
    EXPECT_EQ(image.getPixel(0, 0), ColorB(255, 255, 255, 255));
}

TEST(ImageTest, CopyConstructor)
{
    Image im1{ CatPath };
    Image im2{ im1 };

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
