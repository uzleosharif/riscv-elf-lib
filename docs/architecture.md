

# High-level Architecture

## File Reader

- Responsible for reading the ELF file from disk.
- Handles byte order (endianness) and ensures correct alignment.

## ELF Header Parser

- Parses the ELF header to extract basic information about the file.

## Section and Program Header Parsers

- Parses section and program headers to understand the layout and 
contents of the ELF file.

## Symbol and Relocation Parsers

- Parses symbol tables and relocation entries to interpret the symbols 
and relocations in the ELF file.

## RISC-V Specific Components

- Components that handle RISC-V specific details, such as instruction-decoding,
ABI compliance checks etc.

## CLI and Library Interfaces

- Provides command-line tools for parsing and viewing ELF file contents.
- Optionally, provides a library interface for integration with other tools.
