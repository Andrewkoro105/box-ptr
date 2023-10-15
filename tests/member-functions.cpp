#include "test-classes/test-classes.hpp"

TEST(member_functions, get){
	int* a{new int{}};
	bp::BoxPtr<int> box_ptr{a};
	ASSERT_EQ(box_ptr.get(), a);
}

TEST(member_functions, reset) {
	A::clear();
	A* a{new A{}};
	A* a1{new A{}};
	bp::BoxPtr<A> box_ptr{a};
	box_ptr.reset(a1);
	ASSERT_EQ(A::deleted, 1);
	ASSERT_EQ(a1->get_name(), "A");
}

TEST(member_functions, assignment_operator){
	A::clear();
	A* a {new A {}};
	A* a1 {new A {}};
	bp::BoxPtr<A> box_ptr {a};
	bp::BoxPtr<A> box_ptr1 {a1};
	box_ptr = box_ptr1;
	ASSERT_EQ(A::deleted, 1);
	ASSERT_EQ(A::created, 3);
}

TEST(member_functions, assignment_operator_move){
	A::clear();
	A* a {new A {}};
	A* a1 {new A {}};
	bp::BoxPtr<A> box_ptr {a};
	box_ptr = bp::BoxPtr<A>{a1};
	ASSERT_EQ(A::deleted, 1);
	ASSERT_EQ(A::created, 2);
	ASSERT_EQ(box_ptr.get(), a1);
}

TEST(member_functions, dereferencing_operator) {
	A* a {new A {}};
	bp::BoxPtr<A> box_ptr {a};
	ASSERT_EQ((*box_ptr).get_name(), "A");
	ASSERT_EQ(box_ptr->get_name(), "A");
}