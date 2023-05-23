#include "testClasses/testClasses.hpp"

TEST(memberFunctions, get){
	int* a{new int{}};
	bp::BoxPtr<int> boxPtr{a};
	ASSERT_EQ(boxPtr.get(), a);
}

TEST(memberFunctions, reset) {
	A::clear();
	A* a{new A{}};
	A* a1{new A{}};
	bp::BoxPtr<A> boxPtr{a};
	boxPtr.reset(a1);
	ASSERT_EQ(A::deleted, 1);
	ASSERT_EQ(a1->getName(), "A");
}

TEST(memberFunctions, assignmentOperator){
	A::clear();
	A* a {new A {}};
	A* a1 {new A {}};
	bp::BoxPtr<A> boxPtr {a};
	bp::BoxPtr<A> boxPtr1 {a1};
	boxPtr = boxPtr1;
	ASSERT_EQ(A::deleted, 1);
	ASSERT_EQ(A::created, 3);
}

TEST(memberFunctions, assignmentOperator_move){
	A::clear();
	A* a {new A {}};
	A* a1 {new A {}};
	bp::BoxPtr<A> boxPtr {a};
	boxPtr = bp::BoxPtr<A>{a1};
	ASSERT_EQ(A::deleted, 1);
	ASSERT_EQ(A::created, 2);
	ASSERT_EQ(boxPtr.get(), a1);
}

TEST(memberFunctions, dereferencingOperator) {
	A* a {new A {}};
	bp::BoxPtr<A> boxPtr {a};
	ASSERT_EQ((*boxPtr).getName(), "A");
	ASSERT_EQ(boxPtr->getName(), "A");
}