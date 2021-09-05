#include <iostream>

// Only difference between structs and classes in cpp is that in
// structs members are public while in classes they are private

struct Base {
    // class constructor
    Base() {}
    // ~ indicates the destructor of a class
	virtual ~Base() {}

	virtual void printMe() { std::cout << "Base::printMe\n"; }
};

struct Derived : Base {
	Derived() {}
	virtual ~Derived() {}

	virtual void printMe() { std::cout << "Derived::printMe\n"; }
};

// imagine this is an attacker-created structure in memory
struct Evil {
	Evil() {}
	virtual ~Evil() {}

	virtual void makeAdmin() {
		std::cout << "CFI Prevents this control flow\n";
		std::cout << "Evil::makeAdmin\n";
	}
};

int main(int argc, const char *argv[]) {
	Evil *eptr = new Evil();
	Derived *dptr = new Derived();
    
    // supress warnings of unused arguments
	(void)(argc);
	(void)(argv);

	dptr->printMe();

	// imagine a type confusion vulnerability that does something similar
	dptr = reinterpret_cast<Derived *>(eptr);
	dptr->printMe();

	return 0;
}
