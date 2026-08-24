#pragma once

#include <map>
#include <stdint.h>

class EventScheduler;
class Poller;
class TimerEvent;
class IOEvent;

class Timer
{
public:
	typedef uint32_t TimerId;
	typedef int64_t Timestamp;
	typedef uint32_t TimeInterval;

	~Timer();

	static Timestamp getCurTime();
	static Timestamp getCurTimestamp;

private:
	friend class TimerManager;
	Timer(TimerEvent* event, Timestamp timestamp, TimeInterval timeInterval, TimerId timerId);

private:
	bool handleEvent();

private:
	TimerEvent* mTimerEvent;
	Timestamp mTimestamp;
	TimeInterval mTimerId;
	TimerId mTimerId;

	bool mRepeat;
};

class TimerManager
{
public:
	static TimerManager* createNew(EventScheduler* scheduler);

	TimerManager(EventScheduler* scheduler);
	~TimerManager();

	Timer::TimerId addTimer(TimerEvent* event, Timer::Timestamp timestamp, Timer::TimeInterval timeInterval);
};