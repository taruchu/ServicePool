#pragma once

#include "PoolResourceHandle.h" 
#include "CircularBuffer.h"
#include "LookUpServiceClientBase.h"
#include "PoolServiceFactory.h"
#include <map>

class ResourcePool : public LookUpServiceClientBase //TODO: add concurency later
{
public:
	ResourcePool() { _isInitialized = false; _lookUpService = nullptr; }
	void Initialize(int poolMaximumResources, LookUpServiceBase* lookUpService)
	{
	   if(!_isInitialized)
	   {
		_lookUpService = lookUpService;
		_poolMaximumResources = poolMaximumResources;
		_maxOnDemandAquisition = 0; //TODO: Implement Lazy and Eager Aquisition Pattern
		EagerlyAquireResources(); 
		_isInitialized = true;
	   }
	}
	~ResourcePool() 
	{
		//NOTE: Remember to Clean Up
		_lookUpService = nullptr;
		_resourceTable.clear();
	}
	virtual PoolResourceHandle Aquire(); 
	virtual void Release(int handleID);
	bool PoolIsReady() { return _isInitialized; }
private:
	bool _isInitialized;
	LookUpServiceBase* _lookUpService;
	int _poolMaximumResources, _maxOnDemandAquisition;
	std::map<int, PoolResource> _resourceTable;
	CircularBuffer<int> _freeResourcesBuffer;

	int FindFreeHandleID();
	void RecycleOrEvict(int handleID);
	void EagerlyAquireResources();
	bool VerifyHandleID(int handleID);

	void* AquireResourceProviderByQuery(LookUpServiceBase* service, string property);
	LookUpServiceBase* AquireLookUpServiceAccessPoint(string context);
};