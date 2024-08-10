#include "test-classes/test-classes.hpp"

TEST(member_functions, get){
	auto a{new int{}};
	auto box_ptr{bp::BoxPtr<int>{a}};
	ASSERT_EQ(box_ptr.get(), a);
}

TEST(member_functions, reset) {
	CreateDetector::clear();
	auto a{new CreateDetector{}};
	auto a1{new CreateDetector{}};
	auto box_ptr{bp::BoxPtr<CreateDetector>{a}};
	box_ptr.reset(a1);
	ASSERT_EQ(CreateDetector::deleted, 1);
	ASSERT_EQ(a1->get_name(), "A");
}

TEST(member_functions, assignment_operator){
	CreateDetector::clear();
	auto a {new CreateDetector {}};
	auto a1 {new CreateDetector {}};
	auto box_ptr {bp::BoxPtr<CreateDetector>{a}};
	auto box_ptr1 {bp::BoxPtr<CreateDetector>{a1}};
	box_ptr = box_ptr1;
	ASSERT_TRUE(box_ptr.get() != nullptr);
	ASSERT_TRUE(box_ptr1.get() != nullptr);
	ASSERT_TRUE(box_ptr1.get() != box_ptr.get());
	ASSERT_EQ(CreateDetector::deleted, 1);
	ASSERT_EQ(CreateDetector::created, 3);
}

TEST(member_functions, assignment_operator_move){
	CreateDetector::clear();
	auto a {new CreateDetector {}};
	auto a1 {new CreateDetector {}};
	auto box_ptr {bp::BoxPtr<CreateDetector>{a}};
	box_ptr = bp::BoxPtr<CreateDetector>{a1};
	ASSERT_EQ(CreateDetector::deleted, 1);
	ASSERT_EQ(CreateDetector::created, 2);
	ASSERT_EQ(box_ptr.get(), a1);
}

TEST(member_functions, dereferencing_operator) {
	auto a {new CreateDetector {}};
	auto box_ptr {bp::BoxPtr<CreateDetector>{a}};
	ASSERT_EQ((*box_ptr).get_name(), "A");
	ASSERT_EQ(box_ptr->get_name(), "A");
}