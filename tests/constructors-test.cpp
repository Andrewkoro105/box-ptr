#include "test-classes/test-classes.hpp"

TEST(constructor, base){
	CreateDetector::clear();
	auto b = new KeeperNullBoxBrt();
	ASSERT_EQ(CreateDetector::exists, 1);
	delete b;
	ASSERT_EQ(CreateDetector::exists, 0);
}

TEST(constructor, copy) {
	CreateDetector::clear();
	auto box_ptr{bp::BoxPtr<CreateDetector>{new CreateDetector()}};
	ASSERT_EQ(CreateDetector::exists, 1);
	auto c = new KeeperBoxBrt(box_ptr);
	ASSERT_EQ(CreateDetector::exists, 2);
	delete c;
	ASSERT_EQ(CreateDetector::exists, 1);
	
	auto bp_null{bp::BoxPtr<CreateDetector>{nullptr}};
	auto const& bp_null2{bp_null};
	
	ASSERT_EQ(bp_null2.get(), nullptr);
}

TEST(constructor, move) {
	CreateDetector::clear();
	auto box_ptr0{bp::BoxPtr<CreateDetector>{new CreateDetector()}};
	auto a{box_ptr0.get()};
	
	auto box_ptr1(std::move(box_ptr0));
	ASSERT_EQ(CreateDetector::deleted, 0);
	ASSERT_EQ(CreateDetector::created, 1);
	
	ASSERT_EQ(box_ptr0.get(), nullptr);
	ASSERT_EQ(box_ptr1.get(), a);
}

TEST(constructor, derived_move) {
	CreateDetector::clear();
	auto box_ptr0{bp::BoxPtr<ChildCreateDetector>{new ChildCreateDetector(42)}};
	auto box_ptr1{bp::BoxPtr<CreateDetector>{std::move(box_ptr0)}};
	ASSERT_EQ(CreateDetector::exists, 1);
	ASSERT_EQ(CreateDetector::created, 1);
	ASSERT_EQ(box_ptr1->get_name(), "D");
}

TEST(constructor, make) {
	CreateDetector::clear();
	auto box_ptr{bp::make_box_ptr<CreateDetector>()};
	ASSERT_EQ(CreateDetector::exists, 1);
	ASSERT_EQ(box_ptr->get_name(), "A");
	auto box_ptr1{bp::make_box_ptr<CreateDetector, ChildCreateDetector>(1)};
	ASSERT_EQ(box_ptr1->get_name(), "D");
}
