#pragma once

#include "PoolResource.h" 
class ResourcePool;

class PoolResourceHandle  
{
public:
	PoolResourceHandle(); 
	~PoolResourceHandle();
	PoolResourceHandle& operator=(const PoolResourceHandle &h);
	PoolResourceHandle(const PoolResourceHandle &h);
	PoolResource* operator->();
	void Release();
	bool ValidateHandle();
private:
	void Initialize(ResourcePool &pool, PoolResource &resource);
	PoolResource *_resource; 
 	ResourcePool *_resourcePool; 
	friend ResourcePool;
};