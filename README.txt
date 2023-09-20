----- Yttrius Language Toolkit SDK -----

This is the official repository for the 
kernel of YAPISDK. 

Description:

The Yttrius language (Y++) is a procedural programming 
language that emphasizes programmer autonomy and 
relaxed syntaxing while offering limited support for 
object-oriented programming (OOP) concepts. It heavily 
borrows from C, incorporating features like pointers, 
direct memory access, a concise syntax, a robust type 
system, and declarative constructs.

One of the prominent features of Y++ is its optional 
"caregiver" runtime, which provides runtime optimizations 
such as Just-In-Time (JIT) compilation and advanced 
syscall optimization. JIT compilation dynamically 
translates the source code into native machine code 
during execution, improving performance by capitalizing 
on runtime information. The caregiver runtime's advanced
syscall optimization streamlines the interactions between
the program and the operating system, reducing overhead 
and enhancing efficiency.

Notably, Y++ lacks automatic garbage collection. 
Instead, it offers an inbuilt auto-page mechanism 
within the caregiver runtime. This mechanism efficiently 
manages memory by tracking available memory and allowing 
the programmer to explicitly deallocate memory on demand, 
providing fine-grained control over memory usage.

Y++ serves as the basis for Y++-Exon, a derivative language 
specialized for interfacing between WebAssembly (WASM) 
and ActionScript. Y++-Exon combines the advantages of both 
Y++ and Exon, provide an efficient platform for bridging 
the gap between WASM and ActionScript environments.

Exon, an essential component of Y++-Exon, is an interpreted 
language with JIT compilation support. It can be directly
executed using a virtual machine (VM) integrated into 
Y++. The directive "r@:exon(22, codehere)" triggers a 
separate parser to process Exon code, enabling seamless 
integration between the two languages.

The Exon VM also features garbage collection capabilities, 
efficiently managing memory resources and reducing the 
burden on developers to handle memory deallocation 
manually. Additionally, the VM includes inbuilt 
hardware acceleration for graphical operations, 
enabling faster rendering and computation of graphical elements.

Despite its promising features, the Exon VM is currently 
in an experimental phase, and its upcoming release is 
intended for thorough evaluation and validation. This 
evaluation will provide insights into its real-world 
performance and identify areas for further optimization 
and refinement.

In contrast, StdLangSet adopts a standard node-based 
approach but lacks a graphical user interface (GUI) for 
node interactions. Instead, developers must rely on 
textual descriptions and semantic specifications to 
define nodes, potentially resulting in verbose node 
definitions with numerous attributes and children. 
This approach can lead to code inefficiencies and 
hinder the development process.

To address these issues, languages like Y++ have been 
developed to serve as language sets for converting code
from StdLangSet to a more low-level representation. 
This conversion streamlines the coding process, resulting 
in more concise and efficient programs.

A compiler compiler.

This repository features only official footnotes
for the child language: Y++.

The Yttrius SDK provides the runtime required for
platform independency along with binary generation.
This runtime wrapper is distributed under C-License
and you can modify and redistribute under the terms
of the C-License license agreement.

The Yttrius Language entitles full freedom and control
to the programmer. Without a standard for coding conventions,
memory management, typing system, and default types. Only
limited by that of the lexer to identify and transpile fromat.
The programmer is allowed to modify and redefine the lexer 
within source code, and call compiler and runtime directives 
to configure how they act.

Everything below is detailed under the Yttrius subset language
Y++ which is much more useable under a multiparadigm, high
sugar orientation. Featuring OOP & AOP

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
basic syntax, typing system, and declarative syntax. However,
the benfit of Y++ is that is an option to bundle the program
with a "caregiver" runtime which takes care of the source 
program while it runs. For example, optimizing code (JIT) and
more advanced optimization of syscalls. There is no garbage collection,
but the programmer may use the inbuilt auto-page feature of a caregiving
runtime to keep track of avaliable memory and dealloc memory ondemand.

The Yttrius Language set by itself could be used
as its own language, but the main derivative of it
is Y++. The next being Y++-Exon, mainly used for interfacing
between WASM and ActionScript. Exon is an interpreted language
used primarily in Y++ for JIT compilation. Furthermore, Exon
can directly executed JIT by the Y++ parser by calling the directive
"r@:exon(__22__, codehere)" where "codehere" is treated as a 
separate area for a different parser to be called for.

Exon is directly executed via a virtual machine, featuring garbage
collection and inbuilt hardware acceleration for graphical 
operations. This virtual machine is currently experimental and will
soon be released for evaluation.

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
 
Runtime Environments (Y++):
If the programmer chooses to use a runtime of any kind when bundling their
program, they may choose to compile with a runtime, which only come in 64
bit flavors. Every other environment besides the Virtualized Environment
does not compile to an intermediate paradigm and then executed from there. 
The program all are compiled using the "-reenv" tag in order to expose
cerain functionalities to the runtime to interact between program and runtime
at a native execution speed. Additionally, if the program is run without an environment,
there are severe risks like memory leaks and other memory insecurities that might
take place due to the program itself being very very exposed.

"Caregiver"
The caregiver runtime env. provides memory paging management and JIT
optimizations for linking with any libraries. This is the most simple
and lightweight runtime that can be linked with a Yttrius program.

"GC Runtime"
The GC runtime provides a very basic gabrage collection system and can
be inteprolated with other runtimes. It handles a generational heap of
all avaliable information, where it follows similar guidelines to a JVM
young and old pruning GC. However, due to garbage collection, there is
a Stop the world event ans extra CPU is used when there is a GC event.
This runtime is experimental and should be used with care. It is not
recommended to use this runtime as Y++ already features highly intuitive
memory allocation along with compiler optimized dealloc and free calls.

"Virtualized"
Virtualized runtime provides the programmer with a AOT runtime that can
be able to simulate the behavior of different machine information. For 
example it can simulate different register sizes and bit endianess. 
However due to this high avaliability for customization, many system level
directives are disabled when the compiler is compiled with the top level
argument "-virtual" which forces the compiler to adapt the code for the 
virtualized environment. The compiler thus outputs a special file that
can be specifically read by the desired virtualization runtime. This file format
may vary between vendor, but the code is treated as "bytecode" and features
minimal code optimizations.

Copyright (C) Jack Meng 2023
-------------------------------------------

File types: *.yttr *.ypp *.exol *.yhh *.yxx *.yys

-------------------------------------------


Note: This license has also been called the "New BSD License" or "Modified BSD License". See also the 2-clause BSD License.

Copyright 2023 Jack Meng

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-----------
Eclipse Public License, Version 1.0 (EPL-1.0)

THE ACCOMPANYING PROGRAM IS PROVIDED UNDER THE TERMS OF THIS ECLIPSE PUBLIC LICENSE ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THE PROGRAM CONSTITUTES RECIPIENT'S ACCEPTANCE OF THIS AGREEMENT.

1. DEFINITIONS

"Contribution" means:

    a) in the case of the initial Contributor, the initial code and documentation distributed under this Agreement, and
    b) in the case of each subsequent Contributor:
    i) changes to the Program, and
    ii) additions to the Program;
    where such changes and/or additions to the Program originate from and are distributed by that particular Contributor. A Contribution 'originates' from a Contributor if it was added to the Program by such Contributor itself or anyone acting on such Contributor's behalf. Contributions do not include additions to the Program which: (i) are separate modules of software distributed in conjunction with the Program under their own license agreement, and (ii) are not derivative works of the Program.




     
     
     

"Contributor" means any person or entity that distributes the Program.

"Licensed Patents" mean patent claims licensable by a Contributor which are necessarily infringed by the use or sale of its Contribution alone or when combined with the Program.

"Program" means the Contributions distributed in accordance with this Agreement.

"Recipient" means anyone who receives the Program under this Agreement, including all Contributors.

2. GRANT OF RIGHTS

    a) Subject to the terms of this Agreement, each Contributor hereby grants Recipient a non-exclusive, worldwide, royalty-free copyright license to reproduce, prepare derivative works of, publicly display, publicly perform, distribute and sublicense the Contribution of such Contributor, if any, and such derivative works, in source code and object code form.
    b) Subject to the terms of this Agreement, each Contributor hereby grants Recipient a non-exclusive, worldwide, royalty-free patent license under Licensed Patents to make, use, sell, offer to sell, import and otherwise transfer the Contribution of such Contributor, if any, in source code and object code form. This patent license shall apply to the combination of the Contribution and the Program if, at the time the Contribution is added by the Contributor, such addition of the Contribution causes such combination to be covered by the Licensed Patents. The patent license shall not apply to any other combinations which include the Contribution. No hardware per se is licensed hereunder.
    c) Recipient understands that although each Contributor grants the licenses to its Contributions set forth herein, no assurances are provided by any Contributor that the Program does not infringe the patent or other intellectual property rights of any other entity. Each Contributor disclaims any liability to Recipient for claims brought by any other entity based on infringement of intellectual property rights or otherwise. As a condition to exercising the rights and licenses granted hereunder, each Recipient hereby assumes sole responsibility to secure any other intellectual property rights needed, if any. For example, if a third party patent license is required to allow Recipient to distribute the Program, it is Recipient's responsibility to acquire that license before distributing the Program.
    d) Each Contributor represents that to its knowledge it has sufficient copyright rights in its Contribution, if any, to grant the copyright license set forth in this Agreement.




     
     
     

3. REQUIREMENTS

A Contributor may choose to distribute the Program in object code form under its own license agreement, provided that:

    a) it complies with the terms and conditions of this Agreement; and
    b) its license agreement:
    i) effectively disclaims on behalf of all Contributors all warranties and conditions, express and implied, including warranties or conditions of title and non-infringement, and implied warranties or conditions of merchantability and fitness for a particular purpose;
    ii) effectively excludes on behalf of all Contributors all liability for damages, including direct, indirect, special, incidental and consequential damages, such as lost profits;
    iii) states that any provisions which differ from this Agreement are offered by that Contributor alone and not by any other party; and
    iv) states that source code for the Program is available from such Contributor, and informs licensees how to obtain it in a reasonable manner on or through a medium customarily used for software exchange.




     
     
     

When the Program is made available in source code form:

    a) it must be made available under this Agreement; and
    b) a copy of this Agreement must be included with each copy of the Program.




     
     
     

Contributors may not remove or alter any copyright notices contained within the Program.

Each Contributor must identify itself as the originator of its Contribution, if any, in a manner that reasonably allows subsequent Recipients to identify the originator of the Contribution.

4. COMMERCIAL DISTRIBUTION

Commercial distributors of software may accept certain responsibilities with respect to end users, business partners and the like. While this license is intended to facilitate the commercial use of the Program, the Contributor who includes the Program in a commercial product offering should do so in a manner which does not create potential liability for other Contributors. Therefore, if a Contributor includes the Program in a commercial product offering, such Contributor ("Commercial Contributor") hereby agrees to defend and indemnify every other Contributor ("Indemnified Contributor") against any losses, damages and costs (collectively "Losses") arising from claims, lawsuits and other legal actions brought by a third party against the Indemnified Contributor to the extent caused by the acts or omissions of such Commercial Contributor in connection with its distribution of the Program in a commercial product offering. The obligations in this section do not apply to any claims or Losses relating to any actual or alleged intellectual property infringement. In order to qualify, an Indemnified Contributor must: a) promptly notify the Commercial Contributor in writing of such claim, and b) allow the Commercial Contributor to control, and cooperate with the Commercial Contributor in, the defense and any related settlement negotiations. The Indemnified Contributor may participate in any such claim at its own expense.

For example, a Contributor might include the Program in a commercial product offering, Product X. That Contributor is then a Commercial Contributor. If that Commercial Contributor then makes performance claims, or offers warranties related to Product X, those performance claims and warranties are such Commercial Contributor's responsibility alone. Under this section, the Commercial Contributor would have to defend claims against the other Contributors related to those performance claims and warranties, and if a court requires any other Contributor to pay any damages as a result, the Commercial Contributor must pay those damages.

5. NO WARRANTY

EXCEPT AS EXPRESSLY SET FORTH IN THIS AGREEMENT, THE PROGRAM IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED INCLUDING, WITHOUT LIMITATION, ANY WARRANTIES OR CONDITIONS OF TITLE, NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. Each Recipient is solely responsible for determining the appropriateness of using and distributing the Program and assumes all risks associated with its exercise of rights under this Agreement , including but not limited to the risks and costs of program errors, compliance with applicable laws, damage to or loss of data, programs or equipment, and unavailability or interruption of operations.

6. DISCLAIMER OF LIABILITY

EXCEPT AS EXPRESSLY SET FORTH IN THIS AGREEMENT, NEITHER RECIPIENT NOR ANY CONTRIBUTORS SHALL HAVE ANY LIABILITY FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING WITHOUT LIMITATION LOST PROFITS), HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OR DISTRIBUTION OF THE PROGRAM OR THE EXERCISE OF ANY RIGHTS GRANTED HEREUNDER, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

7. GENERAL

If any provision of this Agreement is invalid or unenforceable under applicable law, it shall not affect the validity or enforceability of the remainder of the terms of this Agreement, and without further action by the parties hereto, such provision shall be reformed to the minimum extent necessary to make such provision valid and enforceable.

If Recipient institutes patent litigation against any entity (including a cross-claim or counterclaim in a lawsuit) alleging that the Program itself (excluding combinations of the Program with other software or hardware) infringes such Recipient's patent(s), then such Recipient's rights granted under Section 2(b) shall terminate as of the date such litigation is filed.

All Recipient's rights under this Agreement shall terminate if it fails to comply with any of the material terms or conditions of this Agreement and does not cure such failure in a reasonable period of time after becoming aware of such noncompliance. If all Recipient's rights under this Agreement terminate, Recipient agrees to cease use and distribution of the Program as soon as reasonably practicable. However, Recipient's obligations under this Agreement and any licenses granted by Recipient relating to the Program shall continue and survive.

Everyone is permitted to copy and distribute copies of this Agreement, but in order to avoid inconsistency the Agreement is copyrighted and may only be modified in the following manner. The Agreement Steward reserves the right to publish new versions (including revisions) of this Agreement from time to time. No one other than the Agreement Steward has the right to modify this Agreement. The Eclipse Foundation is the initial Agreement Steward. The Eclipse Foundation may assign the responsibility to serve as the Agreement Steward to a suitable separate entity. Each new version of the Agreement will be given a distinguishing version number. The Program (including Contributions) may always be distributed subject to the version of the Agreement under which it was received. In addition, after a new version of the Agreement is published, Contributor may elect to distribute the Program (including its Contributions) under the new version. Except as expressly stated in Sections 2(a) and 2(b) above, Recipient receives no rights or licenses to the intellectual property of any Contributor under this Agreement, whether expressly, by implication, estoppel or otherwise. All rights in the Program not expressly granted under this Agreement are reserved.

This Agreement is governed by the laws of the State of New York and the intellectual property laws of the United States of America. No party to this Agreement will bring a legal action under this Agreement more than one year after the cause of action arose. Each party waives its rights to a jury trial in any resulting litigation.
