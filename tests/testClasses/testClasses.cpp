#include "testClasses.hpp"

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

std::string A::getName() {
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


B::B() : boxPtr(new A{}) {
}


C::C(const bp::BoxPtr<A>& boxPtr) : boxPtr(boxPtr) {
}


D::D(int testValue) : testValue(testValue) {}

std::string D::getName() {
	return "D";
}
