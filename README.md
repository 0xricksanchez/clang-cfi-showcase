# Clang Control Flow Integrity Showcase

[Control flow integrity](https://www.microsoft.com/en-us/research/publication/control-flow-integrity/) is an exploit mitigation, like stack cookies, DEP, and ASLR. Like other exploit mitigations, the goal of CFI is to prevent bugs from turning into exploits. CFI works by reducing the ability of an attacker to redirect program execution to an attacker controlled destination.

This repository contains samples with specially crafted bugs to showcase clang's control flow integrity implementation. These examples are an accompaniment to [Trail of Bits' blog post describing CFI in clang](https://blog.trailofbits.com/2016/10/17/lets-talk-about-cfi-clang-edition/).

The bugs in these examples are not statically identified by the compiler, but are detected at runtime via CFI. Where possible, malicious behavior that occurs without CFI protections is simulated.

Each example builds two binaries, one with CFI protection (e.g. `cfi_icall`) and one without CFI protections (e.g. `no_cfi_icall`).

Special thanks to pcc and the LLVM project for the cfi-mfcall sample.

# CFI Examples

* **[icall](source/icall.c)** demonstrates control flow integrity of indirect calls. The example binary accepts a single command line argument (valid values are 0-3, but try invalid values with both binaries!). The command line argument shows different aspects of indirect call CFI protection, or lack thereof.
* **[vcall](source/vcall.cpp)** shows an example of CFI applied to virtual function calls. This example demonstrates how CFI would protect against a type confusion or similar attack.
* **[mfcall](source/mfcall.cpp)** shows clang's protections for indirect calls via member function pointers of the wrong dynamic type. The example binary accepts a single command line argument (valid values are 0-6). The command line arguemnt shows different aspects of this specific type of indirect call CFI protection, or lack thereof.
* **[nvcall](source/nvcall.cpp)** shows clang’s protections for calling non-virtual member functions via something that is not an object that has those functions defined.
* **[unrelated_cast](source/unrelated_cast.cpp)** shows how clang can prevent casts between objects of unrelated types.
* **[derived_cast](source/derived_cast.cpp)** expands on unrelated_cast and shows how clang can prevent casts from an object of a base class to an object of a derived class, if the object is not actually of the derived class.
* **[cast_strict](source/cast_strict.cpp)** showcases the very specific instance where the default level of base-to-derived cast protection, like in derived_cast, would not catch an illegal cast.

# Requirements

Each of these samples have been tested under Ubuntu 18.04 and macOS 10.14 using clang 7 with lld 7, however, any version of clang that supports these CFI schemes and an LTO capable linker should be fine.

I attempted to build and test the samples on Windows 10 as well, but using cmake to target the MSVC ABI with a copy of clang that supports GNU-like command line arguments is currently unsupported. I'll attempt to do this again sometime in the future.

# Building

Short bash scripts are provided to help with compiling the example binaries, cleaning the build directory, and formatting the source files.

Building is just a quick `cmake ../ && make` from the build directory if you'd rather not use the scripts, though that relies on some assumptions about your build environment to work properly
