#pragma once
#include <vector>
#include <queue>
#include <mutex>
#include <stdint.h>
#include "Timer.h"
#include "Event.h"

class Poller;

class EventScheduler
{
public:
	enum PoolerType
	{
		POLLER_SELECT,
		POLLER_POLL,
		POLLER_EPOLL
	};
	static EventScheduler* createNew(PoolerType type);

	explicit EventScheduler(PoolerType type);
	virtual ~EventScheduler();

public:
	bool addTriggerEvent(TriggerEvent * triggerEvent);
	Timer::TimerId addTimedEventRunAfater(TimerEvent* event, Timer::TimeInterval delay);
	Timer::TimerId addTimedEventRunAt(TimerEvent* event, Timer::Timestamp when);
	Timer::TimerId addTimedEventRunEvery(TimerEvent* event, Timer::TimeInterval interval);

	bool removeTimeEvent(Timer::TimerId timerId);
	bool addIOEvent(IOEvent* event);
	bool updateIOEvent(IOEvent* event);
	bool remove(IOEvent* event);

	void loop();

	Poller* poller();
	void setTimerManagerReadCallback(EventCallback cb, void* arg);

private:
	bool mQuit;

};