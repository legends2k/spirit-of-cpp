#include <cstdint>   // ← fixed-width integer types
#include <limits>    // ← query type limits from compiler
#include <optional>
#include <iostream>

std::optional<uintmax_t> ipow2(unsigned pow) {
  // Future-proof by not using uint64_t and limiting to 64-bit architectures.
  // Obtain size from compiler at compile-time; thanks to static typing
  if (pow >= std::numeric_limits<uintmax_t>::digits)
    return {};
                           //                 2⁸  2⁷  2⁶  2⁵  2⁴  2³  2²  2¹
  uintmax_t value = 1;     // ----------------------------------------------+
  value <<= pow;           // … | 0 | 0 | 0 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
  return value;            // ----------------------------------------------+
}

int main(int argc, char** argv) {
  if (argc >= 2) {
    auto const pow = std::strtoul(argv[1], nullptr /*str_end*/, 10 /*base*/);
    auto const result = ipow2(pow);
    if (result)
      std::cout << *result << '\n';
    else
      std::cout << "Power beyond machine limits!\n";
  }
}
