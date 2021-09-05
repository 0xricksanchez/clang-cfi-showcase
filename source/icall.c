#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef int (*int_arg_fn)(int);
typedef int (*float_arg_fn)(float);

static int int_arg(int arg) {
	printf("In %s: (%d)\n", __FUNCTION__, arg);
	return 0;
}

static int float_arg(float arg) {
	printf("CFI should protect transfer to here\n");
	printf("In %s: (%f)\n", __FUNCTION__, (double)arg);
	return 0;
}

static int bad_int_arg(int arg) {
	printf("CFI will not protect transfer to here\n");
	printf("In %s: (%d)\n", __FUNCTION__, arg);
	return 0;
}

static int not_entry_point(int arg) {
	// nop sled for x86 / x86-64
	// these instructions act as a buffer for an indirect control flow transfer to skip
	// a valid function entry point, but continue to execute normal code
	// clang-format off
	 __asm__ volatile (
            "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
            "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
            "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
            "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
            "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
            "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
            "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
            "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
            );
	// clang-format on
	printf("CFI ensures control flow only transfers to potentially valid destinations\n");
	printf("In %s: (%d)\n", __FUNCTION__, arg);
	// need to exit or the program will segfault anyway, since the indirect call skipped the function preamble
	exit(arg);
}

// This defines the struct fields <ret_val> <arr>[size]
struct foo {
	int_arg_fn int_funcs[1];
	int_arg_fn bad_int_funcs[1];
	float_arg_fn float_funcs[1];
	int_arg_fn not_entries[1];
};

// the struct aligns the function pointer arrays so indexing past the end will reliably
// call working function pointers
// Does some "scalar initialization" for struct f 
// https://en.cppreference.com/w/c/language/scalar_initialization
// The expression is evaluated, and its value, after conversion as if by assignment to the type of the object, becomes the initial value of the object being initialized. 
// Scalar variables are used to represent individual fixed-size data objects, such as integers and pointers...
// And we need the scalar init routine to properly fill the array field with the function pointer
// the .fun notation is a C99 feature that allows initialization of struct members by name
// https://en.cppreference.com/w/c/language/struct_initialization
// clang-format off
static struct foo f = {
	.int_funcs = {int_arg},
	.bad_int_funcs = {bad_int_arg},
	.float_funcs = {float_arg},
	.not_entries = {(int_arg_fn)((uintptr_t)(not_entry_point) + 0x20)}
};
// clang-format on

int main(int argc, const char *argv[]) {
	if (argc != 2) {
		printf("Usage: %s <option>\n", argv[0]);
		printf("Option values:\n");
		printf("\t0\tCall correct function\n");
		printf("\t1\tCall the wrong function but with the same signature\n");
		printf("\t2\tCall a float function with an int function signature\n");
		printf("\t3\tCall into the middle of a function\n");
		printf("\n");
		printf("\tAll other options are undefined, but should be caught by CFI :)\n");
		printf("\n\n");
		printf("Here are some pointers so clang doesn't optimize away members of `struct foo f`:\n");
		printf("\tint_funcs: %p\n", (void *)f.int_funcs);
		printf("\tbad_int_funcs: %p\n", (void *)f.bad_int_funcs);
		printf("\tfloat_funcs: %p\n", (void *)f.float_funcs);
		printf("\tnot_entries: %p\n", (void *)f.not_entries);
		return 1;
	}

	printf("Calling a function:\n");

	int idx = argv[1][0] - '0';

	return f.int_funcs[idx](idx);
}

/* no CFI compiled test binary right in main() 
// This shows aus the initialized struct
// The addresses refer to the actual functions so @ 0x5555555552b0 theres int_arg()
pwndbg> p f
$1 = {
  int_funcs = {0x5555555552b0 <int_arg>},
  bad_int_funcs = {0x5555555552e0 <bad_int_arg>},
  float_funcs = {0x555555555320 <float_arg>},
  not_entries = {0x555555555390 <not_entry_point+32>}
}
// Accessing a single field 
pwndbg> p f.int_funcs
$2 = {0x5555555552b0 <int_arg>}
// We can actually acess struct members normally and add an index to it to access other struct member fields..
// index 0 is always the indexed struct field we accessed by its name
// f.int_funcs[0] == f.int_funcs
pwndbg> p f.int_funcs[0]
$7 = (int_arg_fn) 0x5555555552b0 <int_arg>
pwndbg> p f.int_funcs[1]
$8 = (int_arg_fn) 0x5555555552e0 <bad_int_arg>
pwndbg> p f.int_funcs[2]
$9 = (int_arg_fn) 0x555555555320 <float_arg>
pwndbg> p f.int_funcs[3]
$10 = (int_arg_fn) 0x555555555390 <not_entry_point+32>
*/
