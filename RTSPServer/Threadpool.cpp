#include <thread> 
#include <iostream>
#include <mutex>
#include <string>
#include <queue>
#include <condition_variable>
#include <vector>
#include<functional>

class ThreadPool {
	ThreadPool(int numThreads) :stop(false) {
		for (int i = 0; i < numThreads; i++)
		{
			threads.emplace_back([this]
				{
					while (1)
					{
						std::unique_lock<std::mutex> lck(mtx);
						condition.wait(lck, [this] {
							return !tasks.empty() || stop;
						});

						if (stop && tasks.empty())
						{
							return;
						}

						std::function<void()> task(std::move(tasks.front()));
						tasks.pop();
						lck.unlock();
						task();

					}
				});
			
		}
	}

	~ThreadPool()
	{
		{
			std::unique_lock<std::mutex> lck(mtx);
			stop = true;
		}
		condition.notify_all();

		for (auto &t : threads)
		{
			t.join();
		}
	}

	template<class F,class... Arg>
	void enqueue(F&& f, Arg && ... args) {
		std::function<void()> task =
			std::bind(std::forward<F>(f), std::forward<Arg>(args)...);
		{
			std::unique_lock<std::mutex> lck(mtx);
			tasks.emplace(std::move(task));
		}
		condition.notify_one();
	}

private:
	std::vector<std::thread> threads;
	std::queue<std::function<void()>> tasks;
	std::mutex mtx;
	std::condition_variable condition;

	bool stop;
};