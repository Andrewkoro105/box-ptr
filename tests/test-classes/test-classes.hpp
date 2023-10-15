#pragma once

#include <gtest/gtest.h>
#include <box-ptr/BoxPtr.hpp>

class A{
public:
	static int deleted;
	static int created;
	static int exists;
	A();
	
	A(const A& a);
	
	virtual std::string get_name();
	
	~A();
	
	static void clear();
};


class B {
protected:
	bp::BoxPtr<A> box_ptr;

public:
	explicit B();
};


class C {
protected:
	bp::BoxPtr<A> box_ptr;

public:
	explicit C(const bp::BoxPtr<A>& box_ptr);
};


class D : public A{
protected:
	int test_value;
public:
	explicit D(int test_value);
	
	std::string get_name() override;
	
	int get_test_value() const;
};
