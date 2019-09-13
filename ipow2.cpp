#include <cstdint>   // ← fixed width integers
#include <limits>
#include <optional>
#include <iostream>

std::optional<uintmax_t> ipow2(unsigned pow) {
  // evaluated and folded into a constant number at compile-time
  constexpr uint8_t max_digits = std::numeric_limits<uintmax_t>::digits;
  if (pow >= max_digits)
    return {};

  uintmax_t value = 1;
  value <<= pow;
  return value;
}

int main(int argc, char** argv) {
  if (argc >= 2) {
    auto const pow = std::strtoul(argv[1], nullptr, 0);
    auto const result = ipow2(pow);
    if (result)
      std::cout << *result << '\n';
    else
      std::cout << "Power beyond limits!\n";
  }
}
