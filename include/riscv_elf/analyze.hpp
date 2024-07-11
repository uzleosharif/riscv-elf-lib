
// SPDX-License-Identifier: MIT

#pragma once

#include <span>
#include <cstddef>

namespace riscv_elf {

/// @brief Determines the number of instructions from the given RISCV bytecode.
///
/// @param bytecode RISCV bytecode
/// @return The number of instructions contained in the provided bytecode.
auto GetNumberOfInstructions(std::span<std::byte const> bytecode)
    -> std::size_t;

}  // namespace riscv_elf
