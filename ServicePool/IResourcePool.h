#pragma once
#include "ILookUpServiceBase.h"
#include "PoolResourceHandle.h"


class IResourcePool abstract
{
public:
	virtual void Initialize(int poolMaximumResources,
		ILookUpServiceBase* lookUpService,
		int eagerAcquisitionAmount,
		bool lazyAcquisition = false) = 0;
	virtual PoolResourceHandle Acquire() = 0;
	virtual void Release(int handleID) = 0;
	virtual bool PoolIsReady() = 0;
};