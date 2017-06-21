#include "ResourcePool.h" 
#include "Logger.h"
#include <sstream> 

PoolResourceHandle ResourcePool::Aquire()
{
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
		_resourceTable[handleID].Reset();
		_freeResourcesBuffer.Enqueue(handleID);
	}
	return;
}

void ResourcePool::EagerlyAquireResources()
{
	void* provider = AquireResourceProviderByQuery(_lookUpService, "service1234");
	if (provider != nullptr)
	{
		PoolServiceFactory* factory = reinterpret_cast<PoolServiceFactory*>(provider);

		_freeResourcesBuffer.InitializeBuffer(_poolMaximumResources,
			[](unsigned handleID)
			{
				return 0;
			}
		);

		for (int i = 1; i < _poolMaximumResources + 1; i++)
		{
			if (!_freeResourcesBuffer.Full())
			{ 
				_resourceTable[i] = factory->AquirePoolResource1234();
				_resourceTable[i].InitializeServiceResources();
				_resourceTable[i]._handleID = i;
				_freeResourcesBuffer.Enqueue(i);
			}
		}
	} 
	provider = nullptr;
	return;
}

bool ResourcePool::VerifyHandleID(int handleID)
{
	std::map<int, PoolResource>::iterator itr;
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

void * ResourcePool::AquireResourceProviderByQuery(LookUpServiceBase* service, string property)
{
	return service->FirstResourceProviderMatching(property); 
}

LookUpServiceBase * ResourcePool::AquireLookUpServiceAccessPoint(string context)
{
	return nullptr; //NOTE: would use RPC call, but keeping it simple for now
}
