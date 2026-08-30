#include "SelectPoller.h"

SelectPoller::SelectPoller()
{
	FD_ZERO(&mReadSet);
	FD_ZERO(&mWriteSet);
	FD_ZERO(&mExceptionSet);
}

SelectPoller::~SelectPoller()
{

}

SelectPoller* SelectPoller::createNew()
{
	return new SelectPoller();
}

bool SelectPoller::addIOEvent(IOEvent* event)
{
	return updateIOEvent(event);
}

bool SelectPoller::updateIOEvent(IOEvent* event)
{
	int fd = event->getFd();
	if (fd < 0)
	{
		return false;
	}

	FD_CLR(fd, &mReadSet);
	FD_CLR(fd, &mWriteSet);
	FD_CLR(fd, &mExceptionSet);

	IOEventMap::iterator it =  mEventMap.find(fd);
	if (it != mEventMap.end())
	{
		if (event->isReadHandling())
			FD_SET(fd, &mReadSet);
		if (event->isWriteHandling())
			FD_SET(fd, &mReadSet);
		if (event->isErrorHandling())
			FD_SET(fd, &mExceptionSet);
	}
	else
	{
		if (event->isReadHandling())
			FD_SET(fd, &mReadSet);
		if (event->isWriteHandling())
			FD_SET(fd, &mReadSet);
		if (event->isErrorHandling())
			FD_SET(fd, &mExceptionSet);
		mEventMap.insert(std::make_pair(fd, event));
	}

	if (mEventMap.empty())
	{
		mMaxNumSockets = 0;
	}
	else
	{
		mMaxNumSockets = mEventMap.rbegin()->first + 1;
	}

	return true;
}


bool SelectPoller::removeIOEvent(IOEvent* event)
{
	int fd = event->getFd();
	if (fd < 0)
	{
		return 0;
	}

	FD_CLR(fd, &mReadSet);
	FD_CLR(fd, &mWriteSet);
	FD_CLR(fd, &mExceptionSet);

	IOEventMap::iterator it =  mEventMap.find(fd);
	if (it != mEventMap.end())
		mEventMap.erase(it);
	
	if (mEventMap.empty())
		mMaxNumSockets = 0;
	else
		mEventMap.rbegin()->first + 1;

	return true;
}

void SelectPoller::handleEvent() 
{
	fd_set readSet = mReadSet;
	fd_set writeSet = mWriteSet;
	fd_set exceptionSet = mExceptionSet;
	struct timeval timeout;
	int ret;
	int rEvent;

	timeout.tv_sec = 1000;
	timeout.tv_usec = 0;

	ret = select(mMaxNumSockets, &readSet, &writeSet, &exceptionSet, &timeout);

	if (ret < 0) 
	{
		return;
	}
	else 
	{
	}

	for (IOEventMap::iterator it = mEventMap.begin(); it != mEventMap.end(); ++it)
	{
		rEvent = 0;
		if (FD_ISSET(it->first, &readSet))
			rEvent |= IOEvent::EVENT_READ;
		if (FD_ISSET(it->first, &writeSet))
			rEvent |= IOEvent::EVENT_WRITE;
		if (FD_ISSET(it->first, &exceptionSet))
			rEvent |= IOEvent::EVENT_ERROR;

		if (rEvent != 0)
		{
			it->second->setREvent(rEvent);
			mIOEvents.push_back(it->second);
		}
	}

	for (auto& ioEvent: mIOEvents)
	{
		ioEvent->handleEvent();
	}

	mIOEvents.clear();
}