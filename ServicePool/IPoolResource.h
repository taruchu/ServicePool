#pragma once

class IPoolResource abstract
{
public:
	virtual void Service() = 0;  
protected:
	virtual void InitializeServiceResources() = 0;
	virtual void ReleaseServiceResources() = 0;
	unsigned _refCounter;
	unsigned _handleID;
	virtual void Reset() = 0;
	bool _isInitialized; 

	friend class PoolResourceHandle;
	friend class ResourcePool;
};
