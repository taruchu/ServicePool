#pragma once
#include "ILease.h"  

class TimedLease : public ILease
{
public:
	TimedLease();
	~TimedLease();
	bool Initialize(__int64 duration); 
	TimedLease& operator=(const TimedLease &rhs);
	bool LeaseStatus();  
private: 
	__int64 _duration; 
	__int64 _startTime;
	__int64 GetCurrentElapsedTime();
};