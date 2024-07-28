
// SPDX-License-Identifier: MIT

#include "riscv_elf/riscv_elf.hpp"

#include <fstream>
#include <utility>
#include <iterator>
#include <stdexcept>
#include <ios>
#include <print>
#include <format>
#include <algorithm>
#include <ranges>
#include <cstring>

namespace rng = std::ranges;

namespace {

enum class DataPosition {
  kShOffset,
  kShEntSize,
  kShNum,
  kShStrNdx,
  kShOffsetInSectionHeader,
  kShSizeInSectionHeader
};

class InputFileStream final : public std::ifstream {
 public:
  InputFileStream(std::string_view filename,
                  std::ios_base::openmode mode = std::ios_base::in)
      : std::ifstream{std::string{filename}, mode} {}

  auto GetSize() -> std::size_t {
    seekg(0, std::ios::end);
    auto const file_size{tellg()};
    seekg(0, std::ios::beg);
    return static_cast<std::size_t>(file_size);
  }
};

auto constexpr kEiClassPosition{4Z};

template <riscv_elf::EiClass ei_class>
auto GetPosition(DataPosition data_key) -> std::size_t {
  if constexpr (ei_class == riscv_elf::EiClass::k32b) {
    switch (data_key) {
      case DataPosition::kShOffset: {
        return 32Z;
      }
      case DataPosition::kShEntSize: {
        return 46Z;
      }
      case DataPosition::kShNum: {
        return 48Z;
      }
      case DataPosition::kShStrNdx: {
        return 50Z;
      }
      case DataPosition::kShOffsetInSectionHeader: {
        return 16Z;
      }
      case DataPosition::kShSizeInSectionHeader: {
        return 20Z;
      }
      default: {
        throw std::invalid_argument{std::format("Unsupported data_key = {}.",
                                                static_cast<int>(data_key))};
      }
    }
  } else if constexpr (ei_class == riscv_elf::EiClass::k64b) {
    switch (data_key) {
      case DataPosition::kShOffset: {
        return 40Z;
      }
      case DataPosition::kShEntSize: {
        return 58Z;
      }
      case DataPosition::kShNum: {
        return 60Z;
      }
      case DataPosition::kShStrNdx: {
        return 62Z;
      }
      case DataPosition::kShOffsetInSectionHeader: {
        return 24Z;
      }
      case DataPosition::kShSizeInSectionHeader: {
        return 32Z;
      }
      default: {
        throw std::invalid_argument{std::format("Unsupported data_key = {}.",
                                                static_cast<int>(data_key))};
      }
    }
  }

  throw std::runtime_error{
      std::format("Unsupported ei_class = {}", static_cast<int>(ei_class))};
}

template <riscv_elf::EiClass ei_class>
auto inline GetWordSize() -> std::size_t {
  if constexpr (ei_class == riscv_elf::EiClass::k64b) {
    return 8Z;
  } else if constexpr (ei_class == riscv_elf::EiClass::k32b) {
    return 4Z;
  }

  throw std::runtime_error{"Unsupported Elf class."};
}

auto GetByteBufferView(std::span<std::byte const> byte_buffer,
                       std::uint64_t start_of_view_position,
                       std::uint64_t size_of_view)
    -> std::span<std::byte const> {
  return std::span{rng::next(rng::cbegin(byte_buffer),
                             static_cast<std::iter_difference_t<std::uint8_t>>(
                                 start_of_view_position)),
                   size_of_view};
}

auto inline ByteBufferViewToInteger(std::span<std::byte const> byte_buffer_view)
    -> std::uint64_t {
  std::uint64_t data{0};
  std::memcpy(&data, byte_buffer_view.data(), byte_buffer_view.size());
  return data;
}

}  // namespace

namespace riscv_elf {

auto GetRawBytes(std::string_view file_path) -> bytebuffer_t {
  bytebuffer_t byte_buffer{};

  InputFileStream file_stream{file_path, std::ios::binary};
  if (not file_stream.is_open()) {
    throw std::runtime_error(
        std::format("Failed to open the ELF file: {}", file_path));
  }
  auto const file_size{file_stream.GetSize()};
  if (file_size <= 0) {
    throw std::runtime_error{
        "Failed to determine the size of the input file or input file is "
        "empty"};
  }

  byte_buffer.resize(file_size);
  file_stream.read(reinterpret_cast<char*>(byte_buffer.data()),
                   static_cast<std::streamsize>(file_size));
  if (file_stream.fail()) {
    throw std::runtime_error{"Failed to read the file content"};
  }

  return byte_buffer;
}

template <EiClass ei_class>
auto ParseImpl(std::span<std::byte const> byte_buffer) -> ElfContent {
  ElfContent elf_content{.ei_class = ei_class, .text_bytecode = {}};

  // find offset of section-header-table in elf
  auto const sh_offset{ByteBufferViewToInteger(GetByteBufferView(
      byte_buffer, GetPosition<ei_class>(DataPosition::kShOffset),
      GetWordSize<ei_class>()))};
  // find total number of sections
  auto const sh_num{ByteBufferViewToInteger(GetByteBufferView(
      byte_buffer, GetPosition<ei_class>(DataPosition::kShNum), 2))};
  // find size of each such section
  auto const sh_entsize{ByteBufferViewToInteger(GetByteBufferView(
      byte_buffer, GetPosition<ei_class>(DataPosition::kShEntSize), 2))};

  // collect all the section-headers contained in the elf
  auto const section_headers{
      GetByteBufferView(byte_buffer, sh_offset, sh_num * sh_entsize) |
      std::views::chunk(sh_entsize)};

  // find index of section-name-string-table within the section-headers array
  auto const sh_strndx{ByteBufferViewToInteger(GetByteBufferView(
      byte_buffer, GetPosition<ei_class>(DataPosition::kShStrNdx), 2))};
  if (std::cmp_greater_equal(sh_strndx, section_headers.size())) {
    throw std::runtime_error{
        "Invalid offset for section-name-string-table-header within "
        "section-headers."};
  }
  // find the offset of section-name-string-table within the elf
  auto const section_name_string_table_offset{
      ByteBufferViewToInteger(GetByteBufferView(
          section_headers[sh_strndx],
          GetPosition<ei_class>(DataPosition::kShOffsetInSectionHeader),
          GetWordSize<ei_class>()))};

  if (auto const section_text_iter{rng::find_if(
          section_headers,
          [byte_buffer, section_name_string_table_offset](
              auto const& section_header) -> bool {
            auto const sh_name_offset{ByteBufferViewToInteger(
                GetByteBufferView(section_header, 0, 4))};
            std::string_view section_name{reinterpret_cast<char const*>(
                byte_buffer.data() + section_name_string_table_offset +
                sh_name_offset)};
            return (section_name == ".text");
          })};
      section_text_iter == rng::end(section_headers)) {
    std::println("WARN: No .text section found in elf.");
  } else {
    // find the offset of text-section within the elf
    auto const section_text_offset{ByteBufferViewToInteger(GetByteBufferView(
        *section_text_iter,
        GetPosition<ei_class>(DataPosition::kShOffsetInSectionHeader),
        GetWordSize<ei_class>()))};
    // find the size of text-section
    auto const section_text_size{ByteBufferViewToInteger(GetByteBufferView(
        *section_text_iter,
        GetPosition<ei_class>(DataPosition::kShSizeInSectionHeader),
        GetWordSize<ei_class>()))};

    elf_content.text_bytecode.reserve(section_text_size);
    rng::copy(
        GetByteBufferView(byte_buffer, section_text_offset, section_text_size),
        std::back_inserter(elf_content.text_bytecode));
  }

  return elf_content;
}

auto Parse(std::string_view file_path) -> ElfContent {
  auto const byte_buffer{GetRawBytes(file_path)};

  if (auto const ei_class{static_cast<EiClass>(byte_buffer[kEiClassPosition])};
      ei_class == EiClass::k32b) {
    return ParseImpl<EiClass::k32b>(byte_buffer);
  } else if (ei_class == EiClass::k64b) {
    return ParseImpl<EiClass::k64b>(byte_buffer);
  }

  throw std::runtime_error{"Unsupported Elf class."};
}

}  // namespace riscv_elf
