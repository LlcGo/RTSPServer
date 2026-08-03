#include "Thread.h"
#include <iostream>
#include <string>
#include <memory>

class A {
public:
	void testClass()
	{

	}
};

std::thread t2;
void add(int& num)
{
	num += 1;
}

// 2:error
void test()
{
	// 需要变成全局变量
	int num = 0;
	// 能编译但是空指针
	t2 = std::thread(add, std::ref(num));
}

// 


int Day2()
{

	// 1:error
	// std::thread t1(add, 1);

	// 1:success
	int num = 0;
	std::thread t1(add, std::ref(num));

	// 2:error
	// test();

	// 3:在线程里面传入对象的时候要用智能指针
	// 这是错的 -> A a ;

	std::shared_ptr<A> a = std::make_shared<A>();
	std::thread t2(&A::testClass, a);

	t2.join();

	t1.join();
	return 0;
}