#pragma once
#include "IPoolResource.h"
#include "PoolResource.h"
 
class PoolResourceProxy : public IPoolResource
{
public:
	 
	PoolResourceProxy();
	~PoolResourceProxy()
	{
		ReleaseServiceResources();
		return;
	} 
	void Service(IPoolResourceContext* context);
private:
	void InitializeServiceResources();
	void ReleaseServiceResources();
	unsigned _refCounter;
	unsigned _handleID;
	void Reset();
	bool _isInitialized;
	PoolResource* _realThing; //NOTE: I love the band U2 ! :=)

	friend class PoolResourceHandle;
	friend class ResourcePool;
};
