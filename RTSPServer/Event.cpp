#include <stdio.h>

#include "Event.h"

TriggerEvent* TriggerEvent::createNew(void* arg) {
    return new TriggerEvent(arg);
}

TriggerEvent* TriggerEvent::createNew() {
    return new TriggerEvent(NULL);
}

TriggerEvent::TriggerEvent(void* arg) : mArg(arg), mTriggerCallback(NULL) {
}
TriggerEvent::~TriggerEvent() {
}

TriggerEvent* TriggerEvent::createNew(void * arg)
{
    return new TriggerEvent(arg);
}

TriggerEvent* TriggerEvent::createNew()
{
    return new TriggerEvent(NULL);
}

void TriggerEvent::handleEvent()
{
    if (mTriggerCallback)
    {
        mTriggerCallback(mArg);
    }
}

TimerEvent::TimerEvent(void*arg):mTimeoutCallback(NULL),mArg(arg),mIsStop(false){}

TimerEvent::~TimerEvent(){}

TimerEvent* TimerEvent::createNew()
{
    return new TimerEvent(NULL);
}

TimerEvent* TimerEvent::createNew(void * arg)
{
    return new TimerEvent(arg);
}

bool TimerEvent::handleEvent()
{
    if (mIsStop)
    {
        return mIsStop;
    }

    if (mTimeoutCallback)
    {
        mTimeoutCallback(mArg);
    }

    return mIsStop;
}

void TimerEvent::stop()
{
    mIsStop = true;
}

IOEvent* IOEvent::createNew(int fd, void* arg)
{
    if (fd < 0)
        return NULL;
    return new IOEvent(fd, arg);
}

IOEvent* IOEvent::createNew(int fd)
{
    if (fd < 0)
        return NULL;
    return new IOEvent(fd,NULL);
}

IOEvent::IOEvent(int fd, void* arg) :
    mFd(fd),
    mArg(arg),
    mEvent(EVENT_NONE),
    mREvent(EVENT_NONE),
    mReadCallback(NULL),
    mWriteCallback(NULL),
    mErrorCallback(NULL)
{
 }

IOEvent::~IOEvent(){}

void IOEvent::handleEvent()
{
    if (mReadCallback && (mREvent && EVENT_READ))
    {
        mReadCallback(mArg);
    }

    if (mWriteCallback && (mREvent && EVENT_WRITE))
    {
        mWriteCallback(mArg);
    }

    if (mErrorCallback && (mREvent && EVENT_ERROR))
    {
        mErrorCallback(mArg);
    }
}
