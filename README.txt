----- Yttrius WebAPI Interactions SDK -----

This is the official repository for the 
kernel of YAPISDK. 

The Yttrius Language entitles full freedom and control
to the programmer. Without a standard for coding conventions,
memory management, typing system, and default types. Only
limited by that of the lexer to identify and transpile fromat.

[NOTE: Nothing here is finalized nor has anything close to done]

The Yttrius collection includes the following components:
  - The Yttrius Language Set (Y++ & Y-Exon)
  - The Yttrius Compiler & Separate Lexer
  - Default System Static Libraries
  - The Yttrius Build System
  - G-Lib 
  - Default Yttrius STL
  - Builtin Yttrius Runtime Manager
  - Default Yttrius Language Assets (STL)
  - Default Yttrius Builder's Toolchain (WIP)

The Yttrius language (Y++) is a procedural language, that has
minimal support for OOP, while tying in a strong 
focus for overall programmer freedom and complete
lack of strict syntaxing. In many ways it adopts
the many aspects of C, including pointes, complete memory access,
basic syntax, typing system, and declarative syntax.

The language features several variations; however,
many have become obsolete, with the current being Y++.
This standard is mentioned below:

  [!] Y++ -> A high level abstraction of the original
    Yttrius-Exon language. It features structures similar
    to modern day OOP languages like Classes, Objects, 
    Strict Typing, and others. However, it also comes at the
    cost of being very hard to read if used typed incorrectly.

Along with the main language, the SDK features a standard build system 
for the language which is just an abstraction of the original compiler.
The make system, similar to the language's motif, calls for a 
complete lack of proper syntax and lack of memory control to
the system itself. This means the user must allocate his or her
memory accordingly during compilation and control how one file
should be dealt individually, and not calling a func like "compile[]"
on a target structure to compile that code. Instead, the user
must manually link these files together.

Everythign here is WIP (Work in Progress)

This is currently in pre-alpha
Licensed under a custom license

Copyright (C) Jack Meng 2023
-------------------------------------------

File types: *.yttr *.ypp *.exol *.yhh *.yxx *.yys

-------------------------------------------
