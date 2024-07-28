
// SPDX-License-Identifier: MIT

#pragma once

#include <string_view>
#include <vector>
#include <cstddef>

namespace riscv_elf {

using bytebuffer_t = std::vector<std::byte>;
using bytecode_t = bytebuffer_t;

enum class EiClass { kNone, k32b, k64b };

/// @brief Convenient data-structure to hold meaningful information extracted
/// from the ELF file.
struct ElfContent {
  EiClass ei_class{EiClass::kNone};
  bytecode_t text_bytecode;
};

/// @brief Converts the elf file content into the byte-buffer
///
/// @param file_path absolute location of the input ELF file on file-system.
/// @return the byte-buffer containing the ELF blob data.
/// @throws runtime_error
/// @note this does not check the validity of the ELF file. It assumes that
/// the passed-in ELF file was geenrated by a reasonable RISC-V compiler.
auto GetRawBytes(std::string_view file_path) -> bytebuffer_t;

/// @brief Parses the input elf file.
///
/// The input elf file, specified via its location on filesystem, is parsed to
/// extract meaningful information out into a convenient data-structure.
///
/// @param file_path absolute location of the input ELF file on file-system.
/// @return the meaningful data from the ELF file as a data-structure.
/// @throws runtime_error
auto Parse(std::string_view file_path) -> ElfContent;

}  // namespace riscv_elf
