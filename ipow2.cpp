#include <cstdint>   // ← fixed-width integer types
#include <limits>    // ← query type limits from compiler
#include <optional>
#include <iostream>

std::optional<uintmax_t> ipow2(unsigned pow) {
  // Future-proof by not limiting to uint64_t.
  // Constant-time constant queried from compiler; thanks to static typing
  if (pow >= std::numeric_limits<uintmax_t>::digits)
    return {};

  uintmax_t value = 1;
  value <<= pow;
  return value;
}

int main(int argc, char** argv) {
  if (argc >= 2) {
    auto const pow = std::strtoul(argv[1], nullptr /*str_end*/, 10 /*base*/);
    auto const result = ipow2(pow);
    if (result)
      std::cout << *result << '\n';
    else
      std::cout << "Power beyond machine limits!\n";
  } else {
    std::cerr << "Usage: ipow2 POWER";
  }
}
