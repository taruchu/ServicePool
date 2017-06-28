#pragma once
 

class ILeaseGrantor abstract
{
public:
	virtual __int64 AcquireResourceLease(__int64 duration, int handleID = 0) = 0;
	virtual bool EvictResourceLease(__int64 leaseID, int handleID = 0) = 0;
	virtual bool LeaseStatusOfResource(__int64 leaseID, int handleID = 0) = 0;
};