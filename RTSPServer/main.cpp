#include <thread> 
#include <iostream>
#include <mutex>


std::mutex mutx1;
int b = 0;

void func1()
{
	for (int i = 0; i < 1000; i++)
	{
		mutx1.lock();
		b += 1;
		mutx1.unlock();
	}
}


int main()
{
	std::thread t5(func1);
	std::thread t6(func1);
	t5.join();
	t6.join();

	std::cout << b << std::endl;
	
	return 0;
}