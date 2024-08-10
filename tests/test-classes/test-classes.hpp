#pragma once

#include <gtest/gtest.h>
#include <box-ptr/BoxPtr.hpp>

class CreateDetector{
public:
	static int deleted;
	static int created;
	static int exists;
	
	CreateDetector();
	
	CreateDetector(const CreateDetector& a);
	
	virtual std::string get_name();
	
	~CreateDetector();
	
	static void clear();
};

class KeeperNullBoxBrt {
public:
	explicit KeeperNullBoxBrt();
	
protected:
	bp::BoxPtr<CreateDetector> box_ptr;
};


class KeeperBoxBrt {
public:
	explicit KeeperBoxBrt(bp::BoxPtr<CreateDetector> const& box_ptr);

protected:
	bp::BoxPtr<CreateDetector> box_ptr;
};


class ChildCreateDetector : public CreateDetector{
public:
	explicit ChildCreateDetector(int test_value);
	
	std::string get_name() override;
	
	int get_test_value() const;
	
protected:
	int test_value;
};
