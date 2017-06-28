#include "ResourcePool.h" 
#include "Logger.h"
#include <sstream> 
#include <iostream>

ResourcePool::ResourcePool()
{
	_isInitialized = false;
	_lookUpService = nullptr;
	_lazyAcquisition = false;
	return;
}

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

ResourcePool::~ResourcePool()
{
	//NOTE: Remember to Clean Up
	_lookUpService = nullptr; 
	_leasedResourcesTable.clear();
	std::map<int, IPoolResource*>::iterator itr;
	for (itr = _resourceTable.begin(); itr != _resourceTable.end();)
	{
		delete itr->second;
		(itr++)->second = nullptr;
	}
	_resourceTable.clear(); 
	return;
}

PoolResourceHandle ResourcePool::AcquireResource()
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

void ResourcePool::ReleaseResource(int handleID)
{ 
	if (VerifyResourceHandleID(handleID))
	{
		RecycleOrEvict(handleID);
	}
	return;
}

__int64 ResourcePool::AcquireResourceLease(__int64 duration, int handleID)
{
	if (VerifyResourceHandleID(handleID) && _isInitialized)
	{
		__int64 newLeaseID = GetNewIndex();
		TimedLease newLease;
		_leasedResourcesTable[handleID][newLeaseID] = newLease; 
		if (_leasedResourcesTable[handleID][newLeaseID].Initialize(duration))
		{
			return newLeaseID;
		}
	}
	return 0;
}

bool ResourcePool::EvictResourceLease(__int64 leaseID, int handleID)
{
	if (_isInitialized && VerifyLeaseID(handleID, leaseID))
	{ 
		_leasedResourcesTable.erase(handleID);
		cout << "Resource Pool evicted leaseID: " << leaseID << endl;
		return true;
	}
	else if (VerifyLeaseID(handleID, leaseID) == false)
	{
		cout << "Resource Pool cannot evict, it does not own leaseID: " << leaseID << endl;
		return false;
	}
	else
		return false;
}

bool ResourcePool::LeaseStatusOfResource(__int64 leaseID, int handleID)
{
	if (VerifyLeaseID(handleID, leaseID))
	{
		 bool status = _leasedResourcesTable[handleID][leaseID].LeaseStatus();
		 if (!status) 
			 bool evict = EvictResourceLease(leaseID, handleID); 
		  
		return status;
	}
	else
	{
		stringstream stream;
		stream << leaseID;
		Logger log;
		log.LogMessage("Resource Pool Lease status could not find this leaseID: " + stream.str(), __FILE__, __LINE__);
		stream.str();
		return false;
	}		 
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
	//TODO: When should I Evict resource from pool (deallocate)? 
	//Should I decide based on usage stats or use a lower bound allocation amount and just flex around it?
	

	//NOTE: For now just recycle

	if (VerifyLeaseHandleID(handleID))
		_leasedResourcesTable.erase(handleID);

	if (!_freeResourcesBuffer.Full())
	{
		_resourceTable[handleID]->Reset();
		_freeResourcesBuffer.Enqueue(handleID);
		cout << "Resource Pool has recycled handleID: " << handleID << endl;
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

bool ResourcePool::VerifyResourceHandleID(int handleID)
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
		stream.clear();
		return false;
	}
}

bool ResourcePool::VerifyLeaseHandleID(int handleID)
{
	std::map<int, map<__int64, TimedLease>>::iterator itr;
	itr = _leasedResourcesTable.find(handleID);
	if (itr != _leasedResourcesTable.end())
		return true;
	else
	{
		stringstream stream;
		stream << handleID;
		Logger log;
		log.LogMessage("Could not find this lease handleID:" + stream.str(), __FILE__, __LINE__);
		stream.clear();
		return false;
	} 
}

bool ResourcePool::VerifyLeaseID(int handleID,__int64 leaseID)
{ 
	if (VerifyResourceHandleID(handleID) == false)
		return false;

	map<__int64, TimedLease>::iterator itr;
	itr = _leasedResourcesTable[handleID].find(leaseID);
	if (itr != _leasedResourcesTable[handleID].end()) 
		return true; 
	else
	{
		stringstream stream;
		stream << leaseID;
		Logger log;
		log.LogMessage("Could not find this leaseID: " + stream.str(), __FILE__, __LINE__);
		stream.clear();
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

__int64 ResourcePool::GetNewIndex()
{
	return  std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}


void * ResourcePool::AcquireResourceProviderByQuery(ILookUpServiceBase* service, string property)
{
	return service->FirstResourceProviderMatching(property); 
}

ILookUpServiceBase * ResourcePool::AcquireLookUpServiceAccessPoint(string context)
{
	return nullptr; //NOTE: This lookup service is not remote at this time.
}
