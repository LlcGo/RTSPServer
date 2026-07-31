#include "Thread.h"
#include <iostream>
#include <string>

void printfHello(std::string msg)
{
	std::cout << msg << std::endl;
}

int main()
{
	std::thread thread1(printfHello,"hello");
	
	thread1.detach();
	bool isJoin = thread1.joinable();
	if (isJoin)
	{
		thread1.join();
	}

	return 0;
}