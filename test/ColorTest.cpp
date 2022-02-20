#include "ColorMatcher.h"

using namespace testing;

namespace bamboo::test
{
TEST(ColorConversion, ColorB2ColorF_White)
{
    bamboo::ColorB c1{ 255, 255, 255, 255 };
    bamboo::ColorF c2 = bamboo::toColorF(c1);
    EXPECT_THAT(c2, ColorsFEqual(bamboo::ColorF{ 1.0f, 1.0f, 1.0f, 1.0f }));
}

TEST(ColorConversion, ColorB2ColorF_Black)
{
    EXPECT_THAT(bamboo::toColorF(bamboo::ColorB{ 0, 0, 0, 0 }),
                ColorsFEqual(bamboo::ColorF{ 0.0f, 0.0f, 0.0f, 0.0f }));
}

TEST(ColorConversion, ColorB2ColorF_Gray)
{
    EXPECT_THAT(bamboo::toColorF(bamboo::ColorB{ 127, 127, 127, 127 }),
                ColorsFEqual(bamboo::ColorF{ 0.4980f, 0.4980f, 0.4980f, 0.4980f }));
}

TEST(ColorConversion, ColorB2ColorF_DarkRed)
{
    EXPECT_THAT(bamboo::toColorF(bamboo::ColorB{ 100, 23, 34, 254 }),
                ColorsFEqual(bamboo::ColorF{ 0.3921f, 0.0901f, 0.1333f, 0.9960f }));
}

TEST(ColorConversion, ColorF2ColorB_White)
{
    EXPECT_THAT(bamboo::toColorB(bamboo::ColorF{ 1.0, 1.0, 1.0, 1.0 }),
                ColorsBEqual(bamboo::ColorB{ 255, 255, 255, 255 }));
}

TEST(ColorConversion, ColorF2ColorB_Black)
{
    EXPECT_THAT(bamboo::toColorB(bamboo::ColorF{ 0.0, 0.0, 0.0, 0.0 }),
                ColorsBEqual(bamboo::ColorB{ 0, 0, 0, 0 }));
}

}
