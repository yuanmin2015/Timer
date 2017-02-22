#include "stdafx.h"
#include "Timer.h"

Timer::Timer()
{
	timerMap_ = new TimerMap;
	delList_ = new DeleteList;
}

Timer::~Timer()
{
	if (timerMap_ != nullptr)
	{
		TimerMap::iterator iter = timerMap_->begin();
		for (; iter != timerMap_->end(); iter++)
		{
			auto timer = iter->second;
			if (timer != nullptr)
				delete timer;
		}
	}
	
	timerMap_->clear();
	delete timerMap_;
	timerMap_ = nullptr;

	delList_->clear();
}

void Timer::SetTimer(const unsigned int& id, const unsigned int& interval, Action action, void* params /* = nullptr */, const bool& onlyOnce /* = false */)
{
	if (timerMap_->find(id) != timerMap_->end())
		return;

	TimerDefine* timerDef = new TimerDefine;
	timerDef->id = id;
	timerDef->interval = Mill(interval);
	timerDef->action = action;
	timerDef->params = params;
	timerDef->onlyOnce = onlyOnce;

	timerDef->intervalKey = Clock::now();
	SetTimer(timerDef);
}

void Timer::SetTimer(TimerDefine* timerDef)
{
	if (timerDef == nullptr)
		return;
	
	timerMap_->insert(std::make_pair(timerDef->id, timerDef));
}

void Timer::KillTimer(const unsigned int& id)
{
	auto iter = timerMap_->find(id);
	if (iter == timerMap_->end())
		return;

	delete iter->second;
	iter->second = nullptr;
	timerMap_->erase(id);
}

void Timer::After(const unsigned int& id, const unsigned int& interval, Action action, void* params/* = nullptr*/)
{
	SetTimer(id, interval, action, params, true);
}

void Timer::Tick()
{
	for (auto iter = timerMap_->begin(); iter != timerMap_->end(); iter++)
	{
		auto timer = iter->second;
		if (timer != nullptr)
		{
			auto now = Clock::now();
			if (std::chrono::duration_cast<std::chrono::milliseconds>(now - timer->intervalKey) >= timer->interval)
			{
				timer->action(timer->params);
				if (timer->onlyOnce)
				{
					delList_->push_back(timer->id);
				}
				else
				{
					timer->intervalKey = now;
				}
			}
		}
	}

	for each (auto it in *delList_)
	{
		if (timerMap_->find(it) != timerMap_->end())
			timerMap_->erase(it);
	}
	delList_->clear();
}
