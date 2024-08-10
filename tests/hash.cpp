#include <gtest/gtest.h>
#include <box-ptr/BoxPtr.hpp>
#include <string>
#include <array>

TEST(hash, hash) {
	auto test_hash_strings{
		std::array<std::string, 5>{
			"test hash",
			"test hash2",
			"test box ptr hash",
			"test std::hash<bp::BoxPtr<std::string> >",
			"std::hash<bp::BoxPtr<std::string> >"
		}
	};
	for(auto const& str_for_box_ptr: test_hash_strings) {
		for(const auto& str_for_str: test_hash_strings) {
			auto hash_of_box_btr{std::hash<bp::BoxPtr<std::string> >{}(bp::make_box_ptr<std::string>(str_for_box_ptr))};
			auto hash_of_str{std::hash<std::string>{}(str_for_str)};
			ASSERT_EQ(hash_of_box_btr == hash_of_str, str_for_box_ptr == str_for_str);
		}
	}
}