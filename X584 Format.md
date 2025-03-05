# X584 file format

The X584 file format has been created by Arthur Kasimov for his program X584 in 2006.
It is very simple and primitive and contains all necessary data to hold the program
microcode.

## File structure of X584 v1.27 file format

The X584 file starts with magic string `X584` and contains 1024 records of following
format:

- two-byte little-endian opcode which consists of 9-bit opcode for K584VM1
  (for code values see the X584 user manual) and three attribute flags:
  - `ATTR_BREAK == 0x8000`, marks breakpoint;
  - `ATTR_CUSED == 0x2000`, marks usage of input carry bit;
  - `ATTR_CARRY == 0x4000`, marks that input carry bit is 1 if used.

- single-byte value N which contains length of comment string;

- N bytes of comment string in CP1251 encoding. Maximal length is 128 bytes.

## File structure of X584 v2.0 format

Format has been extended by Danila A. Kondratenko with second section starting with
magic string `V2.0` and contains 1024 extended records for each corresponding instruction.

Each record consists of two strings in UTF-8 encoding. First string is control flow
operator string, second string is comment string.

Each string starts with value N holding length of the string in bytes and encoded
in unsigned LEB128 format. The string encoding is specified by TBinaryWriter class
of Delphi Runtime Library.
