#pragma once

#include <gtest/gtest.h>
#include <Box_Ptr/boxPtr.hpp>

class A{
public:
	static int deleted;
	static int created;
	static int exists;
	A();
	
	A(const A& a);
	
	virtual std::string getName();
	
	~A();
	
	static void clear();
};


class B {
protected:
	bp::BoxPtr<A> boxPtr;

public:
	explicit B();
};


class C {
protected:
	bp::BoxPtr<A> boxPtr;

public:
	explicit C(const bp::BoxPtr<A>& boxPtr);
};


class D : public A{
protected:
	int testValue;
public:
	explicit D(int testValue);
	std::string getName() override;
};
