
// SPDX-License-Identifier: MIT

#pragma once

#include <string_view>
#include <vector>
#include <cstddef>

namespace riscv_elf {

using bytecode_t = std::vector<std::byte>;

/// @brief Convenient data-structure to hold meaningful information extracted
/// from the ELF file.
struct ElfContent {
  /// @brief RISC-V machine code as byte-array
  bytecode_t bytecode;
};

/// @brief Parses the input elf file.
///
/// The input elf file, specified via its location on filesystem, is parsed to
/// extract meaningful information out into a convenient data-structure.
///
/// @param file_path absolute location of the input ELF file on file-system.
/// @return the meaningful data from the ELF file as a data-structure.
auto Parse(std::string_view file_path) -> ElfContent;

}  // namespace riscv_elf
