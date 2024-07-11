
// SPDX-License-Identifier: MIT

#include "riscv_elf/analyze.hpp"

namespace riscv_elf {

auto GetNumberOfInstructions(std::span<std::byte const> bytecode)
    -> std::size_t {
  return 0;
}

}  // namespace riscv_elf
