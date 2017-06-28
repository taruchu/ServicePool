#pragma once
 
#include "CircularBuffer.h" 
#include "ILookUpServiceClientBase.h"
#include "IResourcePool.h"
#include "PoolServiceFactory.h"
#include <map>
#include "ILeaseGrantor.h" 
#include "TimedLease.h"
#include <chrono>

class ResourcePool : public IResourcePool, public ILookUpServiceClientBase,
	public ILeaseGrantor //TODO: add concurency later
{
public:
	ResourcePool();
	void Initialize(int poolMaximumResources, 
		ILookUpServiceBase* lookUpService, 
		int eagerAcquisitionAmount, 
		bool lazyAcquisition = false);	
	~ResourcePool();
	
	PoolResourceHandle AcquireResource();
	void ReleaseResource(int handleID);
	bool PoolIsReady() { return _isInitialized; }
	__int64 AcquireResourceLease(__int64 duration, int handleID = 0);
	bool EvictResourceLease(__int64 leaseID, int handleID = 0);
	bool LeaseStatusOfResource(__int64 leaseID, int handleID = 0);

private:
	bool _isInitialized;
	bool _lazyAcquisition;
	ILookUpServiceBase* _lookUpService;
	int _poolMaximumResources, _eagerAcquisitionAmount;
	std::map<int, IPoolResource*> _resourceTable;
	CircularBuffer<int> _freeResourcesBuffer;
	std::map<int, map<__int64, TimedLease>> _leasedResourcesTable; 

	int FindFreeHandleID();
	void RecycleOrEvict(int handleID);
	void EagerlyAcquireResources();
	void LazilyAcquireResources();
	bool VerifyResourceHandleID(int handleID);
	bool VerifyLeaseHandleID(int handleID);
	bool VerifyLeaseID(int handleID, __int64 leaseID);
	bool EmergencyLazyAcquisition();
	bool XpercentFree(float percent);
	__int64 GetNewIndex();

	void* AcquireResourceProviderByQuery(ILookUpServiceBase* service, string property);
	ILookUpServiceBase* AcquireLookUpServiceAccessPoint(string context);
};