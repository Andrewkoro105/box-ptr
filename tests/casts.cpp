#include "test-classes/test-classes.hpp"

TEST(casts, dynamic) {
	bp::BoxPtr<A> box_ptr__a{bp::make_box_ptr<A, D>(34)};
	bp::BoxPtr<D> box_ptr__d{bp::dynamic_box_cast<D>(std::move(box_ptr__a))};
	ASSERT_EQ(box_ptr__d->get_test_value(), 34);
}
