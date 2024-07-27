





# ELF Format 

The ELF is a standard file format for executables, object code, shared libraries, and core dumps.
It is widely used in Unix-like operating systems. Understanding ELF is crucial for tasks like 
parsing binaries, as it allows you to interpret and manipulate the contents of these files.

## Overview of ELF File Structure

An ELF file is composed of several headers and sections, each serving different purposes. The 
main components of an ELF file are:
- ELF Header: Describes the overall file layout.
- Program Header Table: Describes segments for program execution.
- Section Header Table: Describes sections for linking and relocation.
- Sections: Contains various types of data, such as code, data and symbol tables of the binary.

## ELF Header

The ELF Header is at the beginning of the file and contains information that describes the rest 
of the file's structure. (dynamic info)

```
struct ElfHeader {
  unsigned char e_ident[16];  // magic number and other info
  uint16_t e_type;            // object file type
  uint16_t e_machine;         // architecture
  uint32_t e_version;         // 
  // ...
};
```

`e_ident`: identifies the file as an ELF file and provides machine-independent details.
         `EI_MAG0`, `EI_MAG1`, `EI_MAG2`, `EI_MAG3` -> `0x7f`, `E`, `L`, `F`
         `EI_CLASS`: identifies whether the file is 32-bit or 64-bit
         `EI_DATA`: Endianness of the data (little or big-endian)
         `EI_VERSION`: ELF version
         `EI_OSABI`: Target OS ABI
         `EI_ABIVERSION`: ABI version of this ELF


## Program Header Table

## Section Header Table


## References

- Linux manual page on `elf`
