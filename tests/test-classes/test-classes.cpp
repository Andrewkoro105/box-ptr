#include "test-classes.hpp"

int A::deleted = 0;
int A::created = 0;
int A::exists = 0;

A::A() {
	++created;
	++exists;
}

A::A(const A& a) {
	++created;
	++exists;
}

std::string A::get_name() {
	return "A";
}

A::~A() {
	++deleted;
	--exists;
}

void A::clear() {
	ASSERT_EQ(exists, 0);
	deleted = 0;
	created = 0;
}


B::B() : box_ptr(new A{}) {
}


C::C(const bp::BoxPtr<A>& box_ptr) : box_ptr(box_ptr) {
}


D::D(int test_value) : test_value(test_value) {}

std::string D::get_name() {
	return "D";
}

int D::get_test_value() const {
	return test_value;
}
