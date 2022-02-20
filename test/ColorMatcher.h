#pragma once

#include <bamboo/Color.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

constexpr auto COLOR_EPS = 0.0001f;

using namespace testing;

namespace glm
{
void PrintTo(const bamboo::ColorB& value, ::std::ostream* os);
void PrintTo(const bamboo::ColorF& value, ::std::ostream* os);
}

namespace bamboo::test
{

MATCHER_P(ColorsFEqual,
          color,
          std::string((negation ? "doesn't equal" : "equals")) + " to " + PrintToString(color))
{
    return Matches(FloatNear(color.r, COLOR_EPS))(arg.r) &&
           Matches(FloatNear(color.g, COLOR_EPS))(arg.g) &&
           Matches(FloatNear(color.b, COLOR_EPS))(arg.b);
}

MATCHER_P(ColorsBEqual,
          color,
          std::string((negation ? "doesn't equal" : "equals")) + " to " + PrintToString(color))
{
    return color.r == arg.r && color.g == arg.g && color.b == arg.b;
}

}
