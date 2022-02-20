#include "ColorMatcher.h"

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
