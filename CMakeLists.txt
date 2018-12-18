cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_FLAGS "-Weverything -Werror -pedantic -Wno-c++98-compat -Wno-weak-vtables -std=c++14 -O0 -fvisibility=hidden -flto -fno-sanitize-trap=all")
set(CMAKE_C_FLAGS "-Weverything -Werror -pedantic -std=c99 -O0 -fvisibility=hidden -flto -fno-sanitize-trap=all")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/bin")

# CFI enabled builds
project(cfi_cast_strict)
add_executable(cfi_cast_strict cfi_cast_strict.cpp)
target_compile_options(cfi_cast_strict PRIVATE "-fsanitize=cfi-derived-cast" PRIVATE "-fsanitize=cfi-cast-strict")
target_link_options(cfi_cast_strict PRIVATE "-fsanitize=cfi-derived-cast" PRIVATE "-fsanitize=cfi-cast-strict")

project(cfi_derived_cast)
add_executable(cfi_derived_cast cfi_derived_cast.cpp)
target_compile_options(cfi_derived_cast PRIVATE "-fsanitize=cfi-derived-cast")
target_link_options(cfi_derived_cast PRIVATE "-fsanitize=cfi-derived-cast")

project(cfi_icall)
add_executable(cfi_icall cfi_icall.c)
target_compile_options(cfi_icall PRIVATE "-fsanitize=cfi-icall")
target_link_options(cfi_icall PRIVATE "-fsanitize=cfi-icall")

project(cfi_mfcall)
add_executable(cfi_mfcall cfi_mfcall.cpp)
target_compile_options(cfi_mfcall PRIVATE "-fsanitize=cfi-mfcall")
target_link_options(cfi_mfcall PRIVATE "-fsanitize=cfi-mfcall")

project(cfi_nvcall)
add_executable(cfi_nvcall cfi_nvcall.cpp)
target_compile_options(cfi_nvcall PRIVATE "-fsanitize=cfi-nvcall")
target_link_options(cfi_nvcall PRIVATE "-fsanitize=cfi-nvcall")

project(cfi_unrelated_cast)
add_executable(cfi_unrelated_cast cfi_unrelated_cast.cpp)
target_compile_options(cfi_unrelated_cast PRIVATE "-fsanitize=cfi-unrelated-cast")
target_link_options(cfi_unrelated_cast PRIVATE "-fsanitize=cfi-unrelated-cast")

project(cfi_vcall)
add_executable(cfi_vcall cfi_vcall.cpp)
target_compile_options(cfi_vcall PRIVATE "-fsanitize=cfi-vcall")
target_link_options(cfi_vcall PRIVATE "-fsanitize=cfi-vcall")

# CFI disabled builds
project(no_cfi_cast_strict)
add_executable(no_cfi_cast_strict cfi_cast_strict.cpp)
# still use cfi-derived-cast, just not the strict version to show the strict version behavior
target_compile_options(no_cfi_cast_strict PRIVATE "-fsanitize=cfi-derived-cast")
target_link_options(no_cfi_cast_strict PRIVATE "-fsanitize=cfi-derived-cast")

project(no_cfi_derived_cast)
add_executable(no_cfi_derived_cast cfi_derived_cast.cpp)

project(no_cfi_icall)
add_executable(no_cfi_icall cfi_icall.c)

project(no_cfi_mfcall)
add_executable(no_cfi_mfcall cfi_mfcall.cpp)

project(no_cfi_nvcall)
add_executable(no_cfi_nvcall cfi_nvcall.cpp)

project(no_cfi_unrelated_cast)
add_executable(no_cfi_unrelated_cast cfi_unrelated_cast.cpp)

project(no_cfi_vcall)
add_executable(no_cfi_vcall cfi_vcall.cpp)
