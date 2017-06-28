#include "PoolResourceHandle.h"
#include "ResourcePool.h"
#include "Logger.h"
#include <exception>
#include <iostream>

PoolResourceHandle::PoolResourceHandle()
{ 
	_resource = nullptr;
	_resourcePool = nullptr; 
	_leaseID = 0;
	_resourceNullProxy = new PoolResourceNullProxy;
	return;
}

void PoolResourceHandle::Initialize(ResourcePool &pool, IPoolResource *resource)
{
	_resourcePool = &pool;
	_resource = resource;  
	_resource->_refCounter = 1; 
	return;
}

bool PoolResourceHandle::CheckLease()
{ 
	if (_resourcePool == nullptr || _resource == nullptr || _leaseID <= 0)
		return false;
	else
		return _resourcePool->LeaseStatusOfResource(_leaseID, _resource->_handleID);
}

void PoolResourceHandle::GetNewLease(__int64 duration)
{
	if (!CheckLease())
	{   
		_leaseID = _resourcePool->AcquireResourceLease(duration, _resource->_handleID);
		if (_leaseID < 0) 
			_leaseID = 0;
	}
	return;
}

void PoolResourceHandle::EvictLease()
{
	if (CheckLease())
	{
		cout << "Resource Handle evicted leaseID: " << _leaseID << endl;
		if (_resourcePool->EvictResourceLease(_leaseID, _resource->_handleID))
		{ 
			_leaseID = 0; 
		}
	}
	return; 
}

PoolResourceHandle::~PoolResourceHandle()
{
	Release();
	delete _resourceNullProxy;
	return;
}

PoolResourceHandle& PoolResourceHandle::operator=(const PoolResourceHandle &h)
{
	h._resource->_refCounter++;
	Release();
	_resource = h._resource;
	_resourcePool = h._resourcePool; 
	_leaseID = 0;
	return *this;
}

PoolResourceHandle::PoolResourceHandle(const PoolResourceHandle &h)
{
	_resource = h._resource;
	_resourcePool = h._resourcePool;  
	_resource->_refCounter++; 
	_leaseID = 0;
	_resourceNullProxy = new PoolResourceNullProxy;
	return;
}

IPoolResource * PoolResourceHandle::Resource()
{
	if (CheckLease())
		return _resource;
	else
	{
		Release();
		return _resourceNullProxy; //NOTE: Locked out to the real resource... "graceful degradation"
	}
}
		

void PoolResourceHandle::Release()
{
	EvictLease();
	if (_resource != nullptr && _resourcePool != nullptr && --_resource->_refCounter <= 0)
	{ 
		cout << "Resource Handle has released handleID: " << _resource->_handleID << endl;
		_resourcePool->ReleaseResource(_resource->_handleID); //NOTE: Only release to pool when ref count expires.
	}
	_resourcePool = nullptr;
	_resource = nullptr;
	return;
}
 
 
