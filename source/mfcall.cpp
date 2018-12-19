// University of Illinois/NCSA
// Open Source License

// Copyright (c) 2009-2018 by the LLVM project

// All rights reserved.

// Developed by:

//	LLVM Team

//	University of Illinois at Urbana-Champaign

//	http://llvm.org

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal with
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is furnished to do
// so, subject to the following conditions:

//	* Redistributions of source code must retain the above copyright notice,
//		this list of conditions and the following disclaimers.

//	* Redistributions in binary form must reproduce the above copyright notice,
//		this list of conditions and the following disclaimers in the
//		documentation and/or other materials provided with the distribution.

//	* Neither the names of the LLVM Team, University of Illinois at
//		Urbana-Champaign, nor the names of its contributors may be used to
//		endorse or promote products derived from this Software without specific
//		prior written permission.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS WITH THE
// SOFTWARE.

// ==============================================================================

// Copyright (c) 2009-2015 by the LLVM project

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <assert.h>
#include <iostream>
#include <string.h>

struct Foo {
	void b1() { std::cout << "I'm Foo, b1!\n"; }
};

struct Bar {
	void b2() { std::cout << "I'm Bar, b2!\n"; }
};

struct Baz : Foo, Bar {
	virtual ~Baz() {}

	void f1() { std::cout << "I'm Baz, f1!\n"; }
	int f2() {
		std::cout << "I'm Baz, f2!\n";
		return 1;
	}
	virtual void g1() { std::cout << "I'm Baz, g1!\n"; }
	virtual int g2() {
		std::cout << "I'm Baz, g2!\n";
		return 1;
	}
	virtual int g3() {
		std::cout << "I'm Baz, g3!\n";
		return 1;
	}
};

struct Mos {
	virtual ~Mos() {}

	void f1() { std::cout << "I'm Mos, f1!\n"; }
	int f2() {
		std::cout << "I'm Mos, f2!\n";
		return 2;
	}
	virtual void g1() { std::cout << "I'm Mos, g1!\n"; }
	virtual int g2() {
		std::cout << "I'm Mos, g2!\n";
		return 2;
	}
	virtual void g3() { std::cout << "I'm Mos, g3!\n"; }
};

typedef void (Baz::*Baz_void)();
typedef int (Baz::*Baz_int)();
typedef int (Mos::*Mos_int)();

template <typename To, typename From> To bitcast(From f) {
	assert(sizeof(To) == sizeof(From));
	To t;
	memcpy(&t, &f, sizeof(f));
	return t;
}

int main(int argc, char **argv) {
	printf("Calling a function:\n");

	Baz baz;
	Mos mos;

	switch (argv[1][0]) {
	case '0':
		(baz.*bitcast<Baz_int>(&Baz::f1))();
		break;
	case '1':
		(mos.*bitcast<Mos_int>(&Baz::f2))();
		break;
	case '2':
		(baz.*bitcast<Baz_int>(&Baz::g1))();
		break;
	case '3':
		(reinterpret_cast<Baz &>(mos).*&Baz::g2)();
		break;
	case '4':
		(baz.*bitcast<Baz_void>(&Mos::g3))();
		break;
	case '5':
		(baz.*&Foo::b1)();
		break;
	case '6':
		(baz.*&Bar::b2)();
		break;
	default:
		if (argc != 2) {
			printf("Usage: %s <option>\n", argv[0]);
			printf("Option values:\n");
			printf("\t0\tIncorrectly call Baz::f1 from baz object\n");
			printf("\t1\tIncorrectly call Baz::f2 from mos object\n");
			printf("\t2\tIncorrectly call Baz::g1 from baz object\n");
			printf("\t3\tIncorrectly call Baz::g2 from mos object\n");
			printf("\t4\tIncorrectly call Mos::g3 from baz object\n");
			printf("\t5\tCorrectly call Foo::b1 from baz object\n");
			printf("\t6\tCorrectly call Bar::b2 from baz object\n");
			printf("\n\n");
		} else {
			printf("Unknown argument or no arguments given\n");
		}
		break;
	}
}
