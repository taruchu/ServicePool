#pragma once

#include "IPoolResource.h" 
#include "ILeaseHolder.h"
#include "PoolResourceNullProxy.h"
class ResourcePool;

class PoolResourceHandle : public ILeaseHolder //TODO: Make this thread safe
{
public:
	PoolResourceHandle(); 
	~PoolResourceHandle();
	PoolResourceHandle& operator=(const PoolResourceHandle &h);
	PoolResourceHandle(const PoolResourceHandle &h); 
	IPoolResource* Resource();
	void Release();  
	bool CheckLease(); 
	void GetNewLease(__int64 duration); //NOTE: Client must call this and customize the duration
private:
	void Initialize(ResourcePool &pool, IPoolResource* resource); 
	__int64 _leaseID;
	void EvictLease();
	 
	IPoolResource *_resource;
 	ResourcePool *_resourcePool;  
	PoolResourceNullProxy* _resourceNullProxy;
	friend ResourcePool;
};