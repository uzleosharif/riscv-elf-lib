

To build a parser that processes RISC-V binary in ELF format, you need to understand
the structure of ELF (Executable and Linkable Format) files and how to extract 
and interpret RISC-V instructions from these binaries. 

# Understanding ELF Format

- ELF Header: contains metadata about the ELF file
- Program Header Table: describes segments used at runtime
- Section Header Table: contains information about sections (e.g. .text, .data)
- Sections: Different parts of the program like code, data, and symbols

