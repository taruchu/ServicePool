#include "TimedLease.h"
#include <chrono>
#include "Logger.h"

 
TimedLease::TimedLease()
{
	_duration = 0;
	_startTime = 0;
	return;
}
TimedLease::~TimedLease()
{ 
	return;
}
bool TimedLease::Initialize(__int64 duration)
{
	try
	{ 
		_startTime = GetCurrentElapsedTime();
		_duration = duration;
		return true;
	}
	catch (exception ex)
	{
		Logger log;
		log.LogMessage("Timed Lease could not initialize.", __FILE__, __LINE__); 
		return false;
	} 
}
TimedLease & TimedLease::operator=(const TimedLease & rhs)
{
	_duration = rhs._duration;
	_startTime = rhs._startTime;
	return *this;
}

bool TimedLease::LeaseStatus()
{
	__int64 currentTime = GetCurrentElapsedTime();
	if ((currentTime -_startTime) >= _duration)
		return false;
	else
		return true; 
}

__int64 TimedLease::GetCurrentElapsedTime()
{
	return  std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
 
 