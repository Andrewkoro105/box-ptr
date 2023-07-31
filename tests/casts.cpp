#include "testClasses/testClasses.hpp"

TEST(casts, dynamic) {
	bp::BoxPtr<A> boxPtr_A{bp::makeBoxPtr<A, D>(34)};
	bp::BoxPtr<D> boxPtr_D{bp::dynamicCast<D>(std::move(boxPtr_A))};
	ASSERT_EQ(boxPtr_D->getTestValue(), 34);
}
