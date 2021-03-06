#include "PoolResourceProxy.h"
#include <iostream>
using namespace std;

PoolResourceProxy::PoolResourceProxy()
{
	_isInitialized = false;
	_realThing = nullptr; 
	return;
}

void PoolResourceProxy::Service(IPoolResourceContext* context)
{ 
	if (!_isInitialized)
	{
		_realThing = new PoolResource();
		_realThing->InitializeServiceResources(); 
		_isInitialized = true; 
	}  
	_realThing->Service(context);
	cout << "Jesus Loves You so much ... from the proxy :=)" << endl; //Just for fun :=)
	return;
}

void PoolResourceProxy::InitializeServiceResources()
{ 
	//NOTE: Proxy initializes when the service is used.
	return;
}

void PoolResourceProxy::ReleaseServiceResources()
{
	if(_isInitialized)
	{  
		_realThing->ReleaseServiceResources();
		delete _realThing;
		_realThing = nullptr;
		_isInitialized = false;
	}
	return;
}

void PoolResourceProxy::Reset()
{ 
	if (_isInitialized)
	{ 
		_realThing->ReleaseServiceResources();
		delete _realThing;
		_realThing = nullptr;
		_isInitialized = false;
	}
	return;
}
