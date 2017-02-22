#pragma once
#ifndef _TIMER_H_
#define _TIMER_H_

#include <functional>
#include <map>
#include <list>
#include <chrono>

using Action	= std::function<void(void*)>;
using Nano		= std::chrono::nanoseconds;
using Mill		= std::chrono::milliseconds;
using Clock		= std::chrono::high_resolution_clock;
using Duration	= std::chrono::duration<long long, std::milli>;
using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
typedef struct _TTimerDefiner
{
	unsigned int				id			{ 0 };
	Duration					interval;
	TimePoint					intervalKey;
	Action						action		{ nullptr };
	void*						params		{ nullptr };
	bool						onlyOnce	{ false };
}TimerDefine;

class Timer final
{
	using TimerMap = std::map<unsigned int, TimerDefine*>;
	using DeleteList = std::list<unsigned int>;
public:
	Timer();
	~Timer();

public:
	void SetTimer(const unsigned int& id, const unsigned int& interval, Action action, void* params = nullptr, const bool& onlyOnce = false);
	void SetTimer(TimerDefine* timerDef);
	void KillTimer(const unsigned int& id);
	void After(const unsigned int& id, const unsigned int& interval, Action action, void* params = nullptr);
	void Tick();
private:
	TimerMap*		timerMap_	{ nullptr };
	DeleteList*		delList_	{ nullptr };
};
#endif
