#pragma once
 
#include "CircularBuffer.h" 
#include "ILookUpServiceClientBase.h"
#include "IResourcePool.h"
#include "PoolServiceFactory.h"
#include <map>

class ResourcePool : public IResourcePool, public ILookUpServiceClientBase //TODO: add concurency later
{
public:
	ResourcePool() 
	{ 
		_isInitialized = false; 
		_lookUpService = nullptr;
		_lazyAcquisition = false;
		return;
	}
	void Initialize(int poolMaximumResources, 
		ILookUpServiceBase* lookUpService, 
		int eagerAcquisitionAmount, 
		bool lazyAcquisition = false);	
	~ResourcePool() 
	{
		//NOTE: Remember to Clean Up
		_lookUpService = nullptr;
		std::map<int, IPoolResource*>::iterator itr;
		for (itr = _resourceTable.begin(); itr != _resourceTable.end();)
		{
			delete itr->second;
			(itr++)->second = nullptr;
		}
		_resourceTable.clear();
		return;
	}
	PoolResourceHandle Acquire(); 
	void Release(int handleID);
	bool PoolIsReady() { return _isInitialized; }
private:
	bool _isInitialized;
	bool _lazyAcquisition;
	ILookUpServiceBase* _lookUpService;
	int _poolMaximumResources, _eagerAcquisitionAmount;
	std::map<int, IPoolResource*> _resourceTable;
	CircularBuffer<int> _freeResourcesBuffer;

	int FindFreeHandleID();
	void RecycleOrEvict(int handleID);
	void EagerlyAcquireResources();
	void LazilyAcquireResources();
	bool VerifyHandleID(int handleID);
	bool EmergencyLazyAcquisition();
	bool XpercentFree(float percent);

	void* AcquireResourceProviderByQuery(ILookUpServiceBase* service, string property);
	ILookUpServiceBase* AcquireLookUpServiceAccessPoint(string context);
};