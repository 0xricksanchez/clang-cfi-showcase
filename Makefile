CXX = clang++
CC = clang
CFLAGS = -Weverything -Werror -pedantic -std=c99 -O0 -fvisibility=hidden -flto -fno-sanitize-trap=all
CXXFLAGS = -Weverything -Werror -pedantic -Wno-c++98-compat -Wno-weak-vtables -std=c++11 -O0 -fvisibility=hidden -flto -fno-sanitize-trap=all

CFI_TARGETS = cfi_icall cfi_vcall cfi_nvcall cfi_unrelated_cast cfi_derived_cast cfi_cast_strict
NO_CFI_TARGETS = no_cfi_icall no_cfi_vcall no_cfi_nvcall no_cfi_unrelated_cast no_cfi_derived_cast no_cfi_cast_strict

TARGETS = $(CFI_TARGETS) $(NO_CFI_TARGETS)

ifeq ($(OS),Windows_NT)
	detected_OS := Windows
else
	detected_OS := $(shell uname)
endif

all: $(TARGETS)

cfi_icall: cfi_icall.c
	@echo Compiling $<
	@$(CC) $(CFLAGS) -fsanitize=cfi-icall -o ./build/$(detected_OS)/$@ $<

no_cfi_icall: cfi_icall.c
	@echo Compiling $<
	@$(CC) $(CFLAGS) -o ./build/$(detected_OS)/$@ $<

cfi_vcall: cfi_vcall.cpp
	@echo Compiling $<
	@$(CXX) $(CXXFLAGS) -fsanitize=cfi-vcall -o ./build/$(detected_OS)/$@ $<

no_cfi_vcall: cfi_vcall.cpp
	@echo Compiling $<
	@$(CXX) $(CXXFLAGS) -o ./build/$(detected_OS)/$@ $<

cfi_nvcall: cfi_nvcall.cpp
	@echo Compiling $<
	@$(CXX) $(CXXFLAGS) -fsanitize=cfi-nvcall -o ./build/$(detected_OS)/$@ $<

no_cfi_nvcall: cfi_nvcall.cpp
	@echo Compiling $<
	@$(CXX) $(CXXFLAGS) -o ./build/$(detected_OS)/$@ $<

cfi_unrelated_cast: cfi_unrelated_cast.cpp
	@echo Compiling $<
	@$(CXX) $(CXXFLAGS) -fsanitize=cfi-unrelated-cast -o ./build/$(detected_OS)/$@ $<

no_cfi_unrelated_cast: cfi_unrelated_cast.cpp
	@echo Compiling $<
	@$(CXX) $(CXXFLAGS) -o ./build/$(detected_OS)/$@ $<

cfi_derived_cast: cfi_derived_cast.cpp
	@echo Compiling $<
	@$(CXX) $(CXXFLAGS) -fsanitize=cfi-derived-cast -o ./build/$(detected_OS)/$@ $<

no_cfi_derived_cast: cfi_derived_cast.cpp
	@echo Compiling $<
	@$(CXX) $(CXXFLAGS) -o ./build/$(detected_OS)/$@ $<

cfi_cast_strict: cfi_cast_strict.cpp
	@echo Compiling $<
	@$(CXX) $(CXXFLAGS) -fsanitize=cfi-derived-cast -fsanitize=cfi-cast-strict -o ./build/$(detected_OS)/$@ $<

no_cfi_cast_strict: cfi_cast_strict.cpp
	@echo Compiling $<
	@# stil use cfi-derived-cast, just not the strict version to
	@# show the strict version behavior
	@$(CXX) $(CXXFLAGS) -fsanitize=cfi-derived-cast -o ./build/$(detected_OS)/$@ $<

clean:
	rm -f ./build/$(detected_OS)/*

.PHONY: clean all
