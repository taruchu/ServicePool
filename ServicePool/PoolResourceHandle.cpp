#include "PoolResourceHandle.h"
#include "ResourcePool.h"
#include "Logger.h"
#include <exception>

PoolResourceHandle::PoolResourceHandle()
{ 
	_resource = nullptr;
	_resourcePool = nullptr;
}

void PoolResourceHandle::Initialize(ResourcePool &pool, IPoolResource *resource)
{
	_resourcePool = &pool;
	_resource = resource;
	_resource->_refCounter = 1; 
}

PoolResourceHandle::~PoolResourceHandle()
{
	if (_resource != nullptr && _resourcePool != nullptr && --_resource->_refCounter <= 0)
	{
		_resourcePool->Release(_resource->_handleID); 
	}
	_resourcePool = nullptr;
	_resource = nullptr;
}

PoolResourceHandle& PoolResourceHandle::operator=(const PoolResourceHandle &h)
{
	h._resource->_refCounter++;
	if (_resource != nullptr && _resourcePool != nullptr && --_resource->_refCounter <= 0)
	{
		_resourcePool->Release(_resource->_handleID);
		_resourcePool = nullptr;
		_resource = nullptr;
	}
	_resource = h._resource;
	_resourcePool = h._resourcePool;
	return *this;
}

PoolResourceHandle::PoolResourceHandle(const PoolResourceHandle &h)
{
	_resource = h._resource;
	_resourcePool = h._resourcePool;
	_resource->_refCounter++;
}

IPoolResource * PoolResourceHandle::operator->()
{
	try
	{
		return (_resource != nullptr) ? _resource : throw exception("NULL IPoolResource pointer referenced by PoolResourceHandle.");
	}
	catch (exception ex)
	{
		Logger log;
		log.LogMessage(ex.what(), __FILE__, __LINE__);
		exit(1);
	}
}

void PoolResourceHandle::Release()
{
	if (_resource != nullptr && _resourcePool != nullptr && --_resource->_refCounter <= 0)
	{
		_resourcePool->Release(_resource->_handleID); 
	}
	_resourcePool = nullptr;
	_resource = nullptr;
	return;
}

bool PoolResourceHandle::ValidateHandle()
{
	return (_resource != nullptr && _resourcePool != nullptr);
}
