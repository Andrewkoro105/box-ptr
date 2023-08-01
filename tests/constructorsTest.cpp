#include "testClasses/testClasses.hpp"

TEST(constructor, base){
	A::clear();
	B* b = new B();
	ASSERT_EQ(A::exists, 1);
	delete b;
	ASSERT_EQ(A::exists, 0);
}

TEST(constructor, copy) {
	A::clear();
	bp::BoxPtr<A> boxPtr(new A());
	ASSERT_EQ(A::exists, 1);
	C* c = new C(boxPtr);
	ASSERT_EQ(A::exists, 2);
	delete c;
	ASSERT_EQ(A::exists, 1);
	
	auto bpNull{bp::BoxPtr<A>{nullptr}};
	const auto& bpNull2{bpNull};
	
	ASSERT_EQ(bpNull2.get(), nullptr);
}

TEST(constructor, move) {
	A::clear();
	bp::BoxPtr<A> boxPtr0(new A());
	A* a{boxPtr0.get()};
	
	bp::BoxPtr<A> boxPtr1(std::move(boxPtr0));
	ASSERT_EQ(A::deleted, 0);
	ASSERT_EQ(A::created, 1);
	
	ASSERT_EQ(boxPtr0.get(), nullptr);
	ASSERT_EQ(boxPtr1.get(), a);
}

TEST(constructor, make) {
	A::clear();
	bp::BoxPtr<A> boxPtr{bp::makeBoxPtr<A>()};
	ASSERT_EQ(A::exists, 1);
	ASSERT_EQ(boxPtr->getName(), "A");
	bp::BoxPtr<A> boxPtr1{bp::makeBoxPtr<A, D>(1)};
	ASSERT_EQ(boxPtr1->getName(), "D");
	
}