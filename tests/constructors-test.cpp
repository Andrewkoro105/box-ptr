#include "test-classes/test-classes.hpp"

TEST(constructor, base){
	A::clear();
	B* b = new B();
	ASSERT_EQ(A::exists, 1);
	delete b;
	ASSERT_EQ(A::exists, 0);
}

TEST(constructor, copy) {
	A::clear();
	bp::BoxPtr<A> box_ptr(new A());
	ASSERT_EQ(A::exists, 1);
	C* c = new C(box_ptr);
	ASSERT_EQ(A::exists, 2);
	delete c;
	ASSERT_EQ(A::exists, 1);
	
	auto bp_null{bp::BoxPtr<A>{nullptr}};
	const auto& bp_null2{bp_null};
	
	ASSERT_EQ(bp_null2.get(), nullptr);
}

TEST(constructor, move) {
	A::clear();
	bp::BoxPtr<A> box_ptr0(new A());
	A* a{box_ptr0.get()};
	
	bp::BoxPtr<A> box_ptr1(std::move(box_ptr0));
	ASSERT_EQ(A::deleted, 0);
	ASSERT_EQ(A::created, 1);
	
	ASSERT_EQ(box_ptr0.get(), nullptr);
	ASSERT_EQ(box_ptr1.get(), a);
}

TEST(constructor, derived_move) {
	A::clear();
	bp::BoxPtr<D> box_ptr0(new D(42));
	bp::BoxPtr<A> box_ptr1(std::move(box_ptr0));
	ASSERT_EQ(A::exists, 1);
	ASSERT_EQ(A::created, 1);
	ASSERT_EQ(box_ptr1->get_name(), "D");
}

TEST(constructor, make) {
	A::clear();
	bp::BoxPtr<A> box_ptr{bp::make_box_ptr<A>()};
	ASSERT_EQ(A::exists, 1);
	ASSERT_EQ(box_ptr->get_name(), "A");
	bp::BoxPtr<A> box_ptr1{bp::make_box_ptr<A, D>(1)};
	ASSERT_EQ(box_ptr1->get_name(), "D");
}
