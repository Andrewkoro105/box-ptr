#include <iostream>
#include <Box_Ptr/boxPtr.hpp>

class A{
public:
	~A(){
		std::cout << "delete A\n";
	}
};

class B {
protected:
	bp::BoxPtr<A> boxPtr;
	
public:
	explicit B(bp::BoxPtr<A> boxPtr) : boxPtr(new A{*boxPtr.get()}) {
	}
};

int main() {
	B b{bp::makeBoxPtr<A>()};
	return 0;
}
