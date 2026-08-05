#include <thread> 
#include <iostream>
#include <mutex>
#include <string>
#include <condition_variable>
#include <queue>

std::queue<int> queue;
std::mutex mtx3;
std::condition_variable q_cv;
void Product()
{
	for (int i = 0; i < 10; i++)
	{
		std::unique_lock<std::mutex> lock(mtx3);
		queue.push(i);
		q_cv.notify_one();
	}
}

void Consumer()
{
	while (true)
	{
			std::unique_lock<std::mutex> lock(mtx3);
			q_cv.wait(lock, []() {return !queue.empty(); });
			int value = queue.front();
			queue.pop();
	}

}

int main()
{

}