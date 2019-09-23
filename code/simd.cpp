#include <iostream>
#include <array>

// include header from Intel or AMD
#include <emmintrin.h>

constexpr auto COUNT = 4;
using vec4 = std::array<float, COUNT>;

vec4 software() {
  vec4 v1 = {1.0f, 2.0f, 3.0f, 4.0f};
  vec4 v2 = {5.0f, 6.0f, 7.0f, 8.0f};

  vec4 result = {};

  for (int i = 0; i < COUNT; ++i)
    result[i] = v1[i] + v2[i];

  return result;
}

//     +---------------------------+
//     |  1.0 |  2.0 |  3.0 |  4.0 |
//     +---------------------------+
//                 +
//     +---------------------------+
//     |  5.0 |  6.0 |  7.0 |  8.0 |
//     +---------------------------+
//                 =
//     +---------------------------+
//     |  6.0 |  8.0 | 10.0 | 12.0 |
//     +---------------------------+

__m128 hardware() {
  __m128 v1 = _mm_set_ps(1.0f, 2.0f, 3.0f, 4.0f);
  __m128 v2 = _mm_set_ps(5.0f, 6.0f, 7.0f, 8.0f);

  return _mm_add_ps(v1, v2);
}

int main()
{
  vec4 s = software();
  std::cout << "Software: ";
  std::cout << s[0] << '\t'
            << s[1] << '\t'
            << s[2] << '\t'
            << s[3] << "\n\n";

  __m128 h = hardware();
  std::cout << "Hardware: ";
  std::cout << h[3] << '\t'
            << h[2] << '\t'
            << h[1] << '\t'
            << h[0] << "\n\n";
}
