
// SPDX-License-Identifier: MIT

#include <print>
#include <stdexcept>

#include "riscv_elf/riscv_elf.hpp"

auto main() -> int {
  riscv_elf::ElfContent elf_content{};
  try {
    elf_content =
        riscv_elf::Parse("/work/2024/riscv-elf-lib/test/data/test.elf");
  } catch (std::runtime_error const& error) {
    std::println("ERROR: Got exception `{}` while parsing test elf file.",
                 error.what());
  }
}
