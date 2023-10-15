#include <iostream>
#include <Box-Ptr/BoxPtr.hpp>

class A{
public:
	~A(){
		std::cout << "delete A\n";
	}
};

class B {
protected:
	bp::BoxPtr<A> box_ptr;
	
public:
	explicit B(bp::BoxPtr<A> box_ptr) : box_ptr(new A{*box_ptr.get()}) {
	}
};

int main() {
	B b{bp::make_box_ptr<A>()};
	return 0;
}
