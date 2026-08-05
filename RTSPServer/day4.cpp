#include <thread> 
#include <iostream>
#include <mutex>
#include <string>

static Log* log = nullptr;
static std::once_flag once;

class Log {
public:
	Log() {};
	Log(const Log& log) = delete;
	Log& operator=(const Log & log) = delete;

	static Log& GetInstance()
	{
		static Log *log = nullptr;
		// 多线程也不能初始化多个
		std::call_once(once, init);
		return *log;
	}

	static void init() {
		if (!log) log = new Log;
	}

	void PrintLog(std::string msg)
	{
		std::cout << __TIME__ << msg << std::endl;
	}
};

std::mutex mutx2;
int c = 0;

void func3()
{
	for (int i = 0; i < 1000; i++)
	{
		// 自动加锁，自动解锁
		// std::lock_guard<std::mutex> mu(mutx1);

		// 也会自动枷锁，自动解锁
		// std::unique_lock<std::mutex> mu(mutx1);
	
		// 不会自动枷锁 但会自动解锁
		// std::unique_lock<std::mutex> mu(mutx1,std::defer_lock);
		// mu.lock();

		// 延迟5秒后还是获得不到锁就结束返回false
		//std::unique_lock<std::mutex> mu(mutx1,std::defer_lock);
		//mu.try_lock_for(std::chrono::seconds(5));
		c += 1;
	
	}
}


int day4()
{
	Log::GetInstance().PrintLog("error");
	
	return 0;
}