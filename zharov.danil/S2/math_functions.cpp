#include <limits>
#include <stdexcept>
#include "math_functions.hpp"

using lli_t = zharov::lli_t;

lli_t add(const lli_t & a, const lli_t & b)
{
  lli_t max = std::numeric_limits< lli_t >::max();
  lli_t min = std::numeric_limits< lli_t >::min();
  if ((b > 0 && a > max - b) || (b < 0 && a < min - b)){
    throw std::overflow_error("Overflow in add");
  }
  return a + b;
}

lli_t sub(const lli_t & a, const lli_t & b)
{
  lli_t max = std::numeric_limits< lli_t >::max();
  lli_t min = std::numeric_limits< lli_t >::min();
  if ((b > 0 && a < min + b ) || (b < 0 && a > max + b)) {
    throw std::overflow_error("Overflow in sub");
  }
  return a - b;
}

lli_t mult(const lli_t & a, const lli_t & b)
{}

lli_t div(const lli_t & a, const lli_t & b)
{}

lli_t mod(const lli_t & a, const lli_t & b)
{}

lli_t bitShiftLeft(const lli_t & a, const lli_t & b)
{}
