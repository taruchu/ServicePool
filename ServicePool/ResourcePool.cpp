#include "ResourcePool.h" 
#include "Logger.h"
#include <sstream> 

void ResourcePool::Initialize(int poolMaximumResources, 
	ILookUpServiceBase * lookUpService, 
	int eagerAcquisitionAmount, 
	bool lazyAcquisition)
{
	if (!_isInitialized)
	{
		_lookUpService = lookUpService;
		_poolMaximumResources = poolMaximumResources;
		_eagerAcquisitionAmount = (poolMaximumResources >= eagerAcquisitionAmount) ? eagerAcquisitionAmount : poolMaximumResources;
		_lazyAcquisition = lazyAcquisition;
		EagerlyAcquireResources();
		if (_lazyAcquisition)
			LazilyAcquireResources();
		_isInitialized = true;
	}
}

PoolResourceHandle ResourcePool::Acquire()
{
	if (EmergencyLazyAcquisition())
	{
		_lazyAcquisition = true;
		LazilyAcquireResources();
	}  

	unsigned handleID = FindFreeHandleID();
	if (handleID != 0)
	{
		PoolResourceHandle newHandle;
		newHandle.Initialize(*this, _resourceTable[handleID]);
		return newHandle;
	}
	else
		return PoolResourceHandle();
}

void ResourcePool::Release(int handleID)
{ 
	if (VerifyHandleID(handleID))
	{
		RecycleOrEvict(handleID);
	}
	return;
}

int ResourcePool::FindFreeHandleID()
{
	if (!_freeResourcesBuffer.Empty())
	{
		return _freeResourcesBuffer.Dequeue();
	}
	else  
		return 0; 
}

void ResourcePool::RecycleOrEvict(int handleID)
{
	//TODO: Implement Leasing and Evictor Patterns

	//For now just recycle
	if (!_freeResourcesBuffer.Full())
	{
		_resourceTable[handleID]->Reset();
		_freeResourcesBuffer.Enqueue(handleID);
	}
	return;
}

void ResourcePool::EagerlyAcquireResources()
{
	void* provider = AcquireResourceProviderByQuery(_lookUpService, "service1234");
	if (provider != nullptr)
	{
		PoolServiceFactory* factory = reinterpret_cast<PoolServiceFactory*>(provider);
		
		_freeResourcesBuffer.InitializeBuffer(_poolMaximumResources,
			[](unsigned handleID)
			{
				return 0;
			}
		);

		for (int i = 1; i < _eagerAcquisitionAmount + 1; i++)
		{
			if (!_freeResourcesBuffer.Full() && factory != nullptr)
			{ 
				_resourceTable[i] = factory->AcquirePoolResource1234();
				_resourceTable[i]->InitializeServiceResources();
				_resourceTable[i]->_handleID = i;
				_freeResourcesBuffer.Enqueue(i);
			}
		} 
		factory = nullptr;
	}  
	provider = nullptr;
	return;
}

void ResourcePool::LazilyAcquireResources()
{
	void* proxyProvider = (_lazyAcquisition) ? AcquireResourceProviderByQuery(_lookUpService, "service1234_proxy") : nullptr;	 
	if (_lazyAcquisition && proxyProvider != nullptr)
	{
		PoolServiceFactory* proxyFactory =  reinterpret_cast<PoolServiceFactory*>(proxyProvider);
		for (int i = _eagerAcquisitionAmount + 1; i < _poolMaximumResources + 1; i++)
		{
			if (!_freeResourcesBuffer.Full() && proxyFactory != nullptr)
			{
				_resourceTable[i] = proxyFactory->AcquirePoolResource1234Proxy();
				_resourceTable[i]->InitializeServiceResources();
				_resourceTable[i]->_handleID = i;
				_freeResourcesBuffer.Enqueue(i);
			}
		}
		proxyFactory = nullptr;
	}
	proxyProvider = nullptr;
	return;
}

bool ResourcePool::VerifyHandleID(int handleID)
{
	std::map<int, IPoolResource*>::iterator itr;
	itr = _resourceTable.find(handleID);
	if (itr != _resourceTable.end()) 
		return true; 
	else
	{
		std::stringstream stream;
		stream << handleID;
		Logger log; 
		log.LogMessage("Could not find this handleID: " + stream.str(), __FILE__, __LINE__);
		return false;
	}
}

bool ResourcePool::EmergencyLazyAcquisition()
{
	return (_lazyAcquisition == false
		&& XpercentFree(20.0)
		&& (_poolMaximumResources != _eagerAcquisitionAmount));
}

bool ResourcePool::XpercentFree(float percent)
{
	float percentFree = (_freeResourcesBuffer.GetCount() / (_resourceTable.size() * 1.0) ) * 100.0;
	return (percentFree <= percent);
}

void * ResourcePool::AcquireResourceProviderByQuery(ILookUpServiceBase* service, string property)
{
	return service->FirstResourceProviderMatching(property); 
}

ILookUpServiceBase * ResourcePool::AcquireLookUpServiceAccessPoint(string context)
{
	return nullptr; //NOTE: Would use IPC/RPC call, but keeping it simple for now
}
