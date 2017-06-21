#pragma once

#include "IPoolResource.h" 
class ResourcePool;

class PoolResourceHandle  
{
public:
	PoolResourceHandle(); 
	~PoolResourceHandle();
	PoolResourceHandle& operator=(const PoolResourceHandle &h);
	PoolResourceHandle(const PoolResourceHandle &h);
	IPoolResource* operator->();
	void Release();
	bool ValidateHandle();
private:
	void Initialize(ResourcePool &pool, IPoolResource* resource);
	IPoolResource *_resource;
 	ResourcePool *_resourcePool; 
	friend ResourcePool;
};