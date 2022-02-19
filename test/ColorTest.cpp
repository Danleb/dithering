#include <bamboo/Color.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

constexpr auto COLOR_EPS = 0.0001f;

namespace glm
{
void PrintTo(const bamboo::ColorB& value, ::std::ostream* os)
{
    *os << "(" << static_cast<int>(value.r) << ", " << static_cast<int>(value.g) << ", "
        << static_cast<int>(value.b) << ", " << static_cast<int>(value.a) << ")";
}

void PrintTo(const bamboo::ColorF& value, ::std::ostream* os)
{
    *os << "(" << value.r << ", " << value.g << ", " << value.b << ", " << value.a << ")";
}
}

MATCHER_P(ColorsFEqual,
          color,
          std::string((negation ? "doesn't equal" : "equals")) + " to " + PrintToString(color))
{
    return Matches(FloatNear(color.r, COLOR_EPS))(arg.r) &&
           Matches(FloatNear(color.g, COLOR_EPS))(arg.g) &&
           Matches(FloatNear(color.b, COLOR_EPS))(arg.b) &&
           Matches(FloatNear(color.a, COLOR_EPS))(arg.a);
}

MATCHER_P(ColorsBEqual,
          color,
          std::string((negation ? "doesn't equal" : "equals")) + " to " + PrintToString(color))
{
    return color.r == arg.r && color.g == arg.g && color.b == arg.b && color.a == arg.a;
}

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
