#include "Timer.h"
#ifndef _WIN32
#include <sys/timerfd.h>
#endif
#include<chrono>
#include "Event.h"
#include "EnventScheduler.h"
#include "Poller.h"

Timer::~Timer()
{

}

Timer::Timestamp Timer::getCurTime() 
{
#ifndef _WIN32
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (now.tv_sec * 1000 + now.tv_nsec / 1000000);
#else
    long long now = std::chrono::steady_clock::now().time_since_epoch().count();
    return now / 1000000;
#endif // !_WIN32
}

Timer::Timestamp Timer::getCurTimestamp()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();
}

Timer::Timer(TimerEvent* event, Timestamp timestamp, TimeInterval timeInterval, TimerId timerId):
    mTimerEvent(event),
    mTimestamp(timestamp),
    mTimerInterval(timeInterval),
    mTimerId(0)
{
    if (mTimerInterval != 0)
    {
        mRepeat = true;
    }
    else
    {
        mRepeat = false;
    }
}



TimerManager* TimerManager::createNew(EventScheduler* scheduler)
{
    if (!scheduler)
    {
        return nullptr;
    }
    return new TimerManager(scheduler);
}

TimerManager::TimerManager(EventScheduler* scheduler):
    mPoller(scheduler->poller()),mLastTimerId(0)
{
#ifndef _WIN32
    mTimerFd = timerfd_create(CLOCK_MONTONIC, TFD_NONBLOCK | TFD_CLOEXEC);

    if (mTimerFd < 0)
    {
        return;
    }
    else
    {

    }
    mTimerIOEvent = IOEvent::createNew(mTimerFd, this);
    mTimerIOEvent->setReadCallback(readCallback);
    mTimerIOEvent->enableReadHandling();
    modifyTimeout();
    mPoller->addIOEvent(mTimerIOEvent);
#else
    scheduler->setTimerManagerReadCallback(readCallback, this);
#endif // !_WIN32

}

TimerManager::~TimerManager()
{
#ifndef _WIN32
    mPoller->remove(mTimerIOEvent);
    delete mTimerIOEvent;
#endif // !_WIN32
}

Timer::TimerId TimerManager::addTimer(TimerEvent* event, Timer::Timestamp timestamp,
    Timer::TimeInterval timeInterval)
{
    ++mLastTimerId;
    Timer timer( event,timestamp, timeInterval, mLastTimerId);
    mEvents.emplace(timestamp, timer);
    mTimers.emplace(mLastTimerId, timer);
    modifyTimeout();

    return mLastTimerId;
}

bool TimerManager::removeTimer(Timer::TimerId timerId)
{
    std::map<Timer::TimerId, Timer>::iterator it = mTimers.find(timerId);
    if (it != mTimers.end())
    {
        mTimers.erase(it);
    }

    modifyTimeout();
}

void TimerManager::readCallback(void* arg)
{
    TimerManager* timeM = (TimerManager*)arg;
    timeM->handleRead();
}

void TimerManager::handleRead()
{
    if (mTimers.empty() && mEvents.empty())
    {
        return;
    }

    Timer::Timestamp timestamp =  Timer::getCurTime();

    std::multimap<Timer::Timestamp, Timer>::iterator it = mEvents.begin();
    Timer timer = it->second;
    int expire = timer.mTimestamp - timestamp;

    if (expire == 0 || timestamp > timer.mTimestamp)
    {
        bool timerEventStop = timer.handleEvent();
        mEvents.erase(it);
        if (timer.mRepeat)
        {
            if (timerEventStop)
            {
                mTimers.erase(timer.mTimerId);
            }
            else
            {
                timer.mTimestamp = timestamp + timer.mTimerInterval;
                mEvents.insert(std::make_pair(timer.mTimestamp, timer));
            }
        }
        else
        {
            mTimers.erase(timer.mTimerId);
        }
    }
    modifyTimeout();
}

void TimerManager::modifyTimeout()
{
#ifndef _WIN32
    std::multimap<Timer::Timestamp, Timer>::iterator it = mEvents.begin();
    if (it != mEvents.end())
    {
        Timer timer = it->second;
        timerFdSetTime(mTimerFd, timer.mTimestamp, timer.mTimeInterval);
    }
    else 
    {
        timerFdSetTime(mTimerFd, 0, 0);
    }
#endif // !WIN32

}