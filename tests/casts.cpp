#include "test-classes/test-classes.hpp"
#include <variant>

TEST(casts, dynamic) {
	auto box_ptr_a{bp::make_box_ptr<CreateDetector, ChildCreateDetector>(34)};
	auto box_ptr_d{bp::dynamic_box_cast<ChildCreateDetector>(std::move(box_ptr_a))};
	ASSERT_EQ(box_ptr_d->get_test_value(), 34);
}
