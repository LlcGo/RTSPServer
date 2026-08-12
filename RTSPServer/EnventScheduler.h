#pragma once

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
	bool addTriggerEvent();


};