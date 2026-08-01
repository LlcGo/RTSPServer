#include "Thread.h"
#include <iostream>
#include <string>
#include <memory>

class A {
public:
	int a;
};

std::thread t2;
void add(int &num)
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
void testClass(A& a)
{

}

int main()
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
	std::thread t2(testClass, &a);

	t1.join();
	return 0;
}