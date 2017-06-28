#pragma once
#include "IPoolResource.h"

 
class PoolResource : public IPoolResource
{
public: 
	virtual void Service(IPoolResourceContext* context);
	PoolResource() { _isInitialized = false; } 
	virtual ~PoolResource()
	{
		ReleaseServiceResources();
	}
protected:
	virtual void InitializeServiceResources();
	virtual void ReleaseServiceResources();
	unsigned _refCounter;
	unsigned _handleID;
	virtual void Reset();
	bool _isInitialized; 

	friend class PoolResourceProxy;
	friend class PoolResourceHandle;	 
	friend class ResourcePool;
};
