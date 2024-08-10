#include "test-classes.hpp"

int CreateDetector::deleted = 0;
int CreateDetector::created = 0;
int CreateDetector::exists = 0;

CreateDetector::CreateDetector() {
	++created;
	++exists;
}

CreateDetector::CreateDetector(const CreateDetector& a) {
	++created;
	++exists;
}

std::string CreateDetector::get_name() {
	return "A";
}

CreateDetector::~CreateDetector() {
	++deleted;
	--exists;
}

void CreateDetector::clear() {
	ASSERT_EQ(exists, 0);
	deleted = 0;
	created = 0;
}


KeeperNullBoxBrt::KeeperNullBoxBrt() : box_ptr(new CreateDetector{}) {
}


KeeperBoxBrt::KeeperBoxBrt(bp::BoxPtr<CreateDetector> const& box_ptr) : box_ptr(box_ptr) {
}


ChildCreateDetector::ChildCreateDetector(int test_value) : test_value(test_value) {}

std::string ChildCreateDetector::get_name() {
	return "D";
}

int ChildCreateDetector::get_test_value() const {
	return test_value;
}
