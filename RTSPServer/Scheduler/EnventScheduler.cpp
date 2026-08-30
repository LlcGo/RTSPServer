#include "EnventScheduler.h"
#include "SocketsOps.h"
#include "SelectPoller.h"

#ifndef WIN32
#include <sys/eventfd.h>
#endif // WIN32


EventScheduler* EventScheduler::createNew(PoolerType type)
{
	if (type != POLLER_SELECT && type != POLLER_POLL && type != POLLER_EPOLL)
	{
		return nullptr;
	}

	return new EventScheduler(type);
}


EventScheduler::EventScheduler(PoolerType type) :mQuit(false)
{
	if (type == POLLER_SELECT)
	{
		mPoller = SelectPoller::createNew();
	}
	else {
		exit(-1);
		return;
	}

	mTimerManager = TimerManager::createNew(this);
}

EventScheduler::~EventScheduler()
{
	delete mTimerManager;
	delete mPoller;
}

bool EventScheduler::addTriggerEvent(TriggerEvent* triggerEvent)
{
	mTriggerEvents.push_back(triggerEvent);

	return;
}

Timer::TimerId EventScheduler::addTimedEventRunAfater(TimerEvent* event, Timer::TimeInterval delay)
{
	Timer::Timestamp runTime = Timer::getCurTime();
	runTime += delay;
	return mTimerManager->addTimer(event, runTime, 0);
}

Timer::TimerId EventScheduler::addTimedEventRunAt(TimerEvent* event, Timer::Timestamp when)
{
	return mTimerManager->addTimer(event, when, 0);
}

Timer::TimerId EventScheduler::addTimedEventRunEvery(TimerEvent* event, Timer::TimeInterval interval)
{
	Timer::Timestamp runTime = Timer::getCurTime();
	runTime += interval;
	return mTimerManager->addTimer(event, runTime, interval);
}


bool EventScheduler::addIOEvent(IOEvent* event)
{
	return mPoller->addIOEvent(event);
}

bool EventScheduler::updateIOEvent(IOEvent* event)
{
	return mPoller->updateIOEvent(event);
}

bool EventScheduler::removeIOEvent(IOEvent* event)
{
	return mPoller->removeIOEvent(event);
}


void EventScheduler::loop()
{
#if _WIN32

	std::thread([](EventScheduler* esd) {
		while (!esd->mQuit)
		{
			if (esd->mTimerManagerReadCallback)
			{
				esd->mTimerManagerReadCallback(esd->mTimerManagerArg);
			}
		}
		},this).detach();
#endif // _WIN32
		while (!mQuit)
		{
			handleTriggerEvents();
			mPoller->handleEvent();
         }
}

void EventScheduler::handleTriggerEvents()
{
	if (!mTriggerEvents.empty())
	{
		for (std::vector<TriggerEvent*>::iterator it = mTriggerEvents.begin(); it != mTriggerEvents.end(); it++)
		{
			(*it)->handleEvent();
		}
		mTriggerEvents.clear();
	}
}

Poller* EventScheduler::poller()
{
	return mPoller;
}

void EventScheduler::setTimerManagerReadCallback(EventCallback cb, void* arg)
{
	mTimerManagerReadCallback = cb;
	mTimerManagerArg = arg;
}
