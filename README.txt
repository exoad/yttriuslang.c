----- Yttrius WebAPI Interactions SDK -----

This is the official repository for the 
kernel of YAPISDK. 

This repository features only official footnotes
for the child language: Y++.

The Yttrius SDK provides the runtime required for
platform independency along with binary generation.
This runtime wrapper is distributed under GPL-3.0
and you can modify and redistribute under the terms
of the GPL-3.0 license agreement.

The Yttrius Language entitles full freedom and control
to the programmer. Without a standard for coding conventions,
memory management, typing system, and default types. Only
limited by that of the lexer to identify and transpile fromat.
The programmer is allowed to modify and redefine the lexer 
within source code, and call compiler and runtime directives 
to configure how they act.

Everything below is detailed under the Yttrius subset language
Y++ which is much more useable.

This language and all of its subsets and adaptations are not
meant to replace any other languages. Instead, it is best
to use other language with much more defined guidelines. This
language and its bundled lexer and compiler provides no safe
guard on how things are compiled and lexed. The best use case
would be for an interface with other toolkits and languages.
The entry complexity for this language is also very high,
even me, the writer, does not know the full potential to just
calculute relations between complex numbers without falling
down a fault. It is complex for high freedom to the user to
modify the lowest level of software with ease.

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

However, a simple Y++ code to add two numbers together could be as simple as:

"""
imports -> {
   maths@arth -> 0 // include standard math arithmetics in order to not rely on pure bitwise 
}

main(<int32:varags> x[]): // standard compliant argument listing
   int32 &t = make -> x:0 + x:1 // array acessing of the provided runtime arguments
   exit ((t)&0xFF) // exit the program (in theory prints, however, for IO, stdIO should be used) the value
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
  - During runtime, the programmer may request for a memory page, in
  which memory is brought in from the hard drive and used temporarily.
  This in turn means that the runtime cannot check for any validity
  or perform any memory compression. This process only returns a single
  memory bloc to the programmer. Another downside is that the memory 
  paging process is completely black boxed from the programmer. The 
  programmer can suggest a buffering strategy and paging strategy 
  through a dynamic enum defining the standard strategies, but the 
  runtime can ignore this (and will most likely override this call).
  - Memory paging also takes significant CPU overhead and may cause 
  dramatic memory vulns including the fact that the programmer must keep
  track of everything including calling the operating system directly
  for a process id header for the memory allocated. This in turn dramatically
  ruins the cross platform ability of this language.
  - Native compiled applications do not have valid runtime calls; in which these 
  functions that were once allocated to the runtime are now nullable. 
  No errors are thrown but may cause errors because the functions returns a 
  random memory address. The toolkit provides a standard directive that 
  throws an error during native runtime if the requested function is processed
  as undefined (ONLY RUNTIME COMPLIANT). This can be achieved by 
  multiple implemented functions in the toolkit, but the most suitable
  and compliant will be found "__stl_yttrius_sysu.yxh"'s __y_kick_
  which can be called with 1 argument or none. This argument of type:
  "TT_YXX_strate_nameq” which is used as an allocator for a standard 
  strategy that is suggested to be compiled as a specific paging callback.
 

Copyright (C) Jack Meng 2020
-------------------------------------------

File types: *.yttr *.ypp *.exol *.yhh *.yxx *.yys

-------------------------------------------
