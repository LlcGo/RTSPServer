#pragma once

typedef void (*EventCallback)(void*);

class TriggerEvent {
public:
	static TriggerEvent* createNew(void*arg);
	static TriggerEvent* createNew();

	TriggerEvent(void* arg);
	~TriggerEvent();

	void setArg(void* arg);
	void setTriggerCallback(EventCallback cb) {

	}
private:
	void* mArg;
	EventCallback mTriggerCallback;
};

class TimerEvent {

};