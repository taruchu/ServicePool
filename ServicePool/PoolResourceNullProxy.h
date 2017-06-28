#pragma once
#include "IPoolResource.h"

class PoolResourceNullProxy : public IPoolResource //NOTE: This class is used for graceful degradation
{
public:
	void Service(IPoolResourceContext* context) {}
protected:
	void InitializeServiceResources() {}
	void ReleaseServiceResources(){}
	unsigned _refCounter;
	unsigned _handleID;
	void Reset(){}
	bool _isInitialized;

	friend class PoolResourceHandle; 
};