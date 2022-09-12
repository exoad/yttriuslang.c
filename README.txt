----- Yttrius WebAPI Interactions SDK -----

This is the official repository for the 
kernel of YAPISDK. 

The Yttrius SDK provides the runtime required for
platform independency along with binary generation.
This runtime wrapper is distributed under GPL-3.0
and you can modify and redistribute under the terms
of the GPL-3.0 license agreement.

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

The Yttrius Language set by itself could be used
as its own language, but the main derivative of it
is Y++. The next being Y++-Exon, mainly used for interfacing
between WASM and ActionScript.

StdLangSet is a standard node based language; however,
by default does not feature a GUI based node interface.
Instead the user must rely on complete wording and
semantics to describe a node. This in turn can cause
defining a single node (node_object_i) to have 
thousands of compiled attributes and children, which
is very very inefficient. This is why langset adaptations
like Y++ exists on-top as a language set to convert
code from one lang to the much more low level code.

A simple StdLangSet snippet can be:

"""
forma:default_struct [stl_node, stl_RTV ]::0 -> {
  #__name: "My Node :)":str_ascii % an optional default attribute required for all declared nodes
  (:${forma::std::write_in}(&__name:))->[::4]
  (:)->(:??)[{ % a list of random attribute. the ":??" asserting that at least one attribute will be online or be retreivable by any congruent node
    RTV ->[0(integer)] "DEFAULT":str_ascii=>"STD":str_ascii, % assignment of attributes using the "=>" operator to denote flow
    RTV ->[1(integer)] "DEFAULT_1":str_ascii=>"STD_1":str_ascii, % assignment must have proper indexing and this can be painful if u have a lot
    RTV ->[2(integer)] "DEFAULT_2":str_ascii=>"SD_2":str_ascii, % there are no static typing, all non keyword operators must be casted or declared
  }]:[args:~>1] % at most one sub child node can inherit these attributes
  link: __default__ % a required attribute to declare visibility
  ... % additional stuffs maybe
}

forma?->[##__name]->func:print_some_where % this part won't work as i didn't wanna write a whole output section, but this is a general 
                                         % exection flow code snippet where the "->" single flow operator is used to denote how
                                         % data flows but in particular does not do automatic type guessing or type checking until
                                         % runtime
"""

Execution & Compilation Arch.:
  - Primarily the standard SDK bundles a compiler
  that transpiles a child language to the compatible
  Yttrius LangSet and then compiles to a binary image.
  This binary image in turn can be run across any platform
  supported by that runtime or higher.
  - Transpilation is done by comparing the language definitions
  from the StdLangSet to that child's LangSet and then swapping
  definitions. This step does not include tiered compilation &
  preprocessing optimizations.
  - Compilation is done by generating a runtime image (*.yio)
  and then using the StdLangSet's definitions to convert it
  to the runtime readable assembler & lexer. On average, compilation
  is heavily restrictive and has hard limits for how much memory
  it can use (cap be taken off with "-m_cap:0"). The standard
  being 256mB and uses the best pedantic buffering strategies to 
  produce the final image.
  - The SDK's compiler also features compiling to a native executable;
  however this option does not link at compilation, and the user must
  generate the proper static and dynamic libraries and link them by
  themselves. This feature also eliminates a proper filesystem API, thus
  making the user rely on the much more low level Stl::File_System
  node family.
  - Runtime is not in the form a JVM, instead, the runtime
  provides necessary native call simplifications along with
  the default filesystem API. There are no garbage collection,
  nor any memory safety, and the programmer must manage the memory
  entirely on their own. A hardbound memory cap can be set for a
  runtime image; however this can usually cause hard crashes on the 
  runtime and/or non-traceable faults.
  - Runtime calls are placed into a queue where calls are not
  async and will be evaled based on their position within this
  queue. This restriction is to prevent overhead on the runtime's
  filesystem API which was developed with single threaded applications
  in mind.

Copyright (C) Jack Meng 2020
-------------------------------------------

File types: *.yttr *.ypp *.exol *.yhh *.yxx *.yys

-------------------------------------------
