#include "PoolServiceFactory.h"

PoolServiceFactory::PoolServiceFactory(ILookUpServiceBase* lookUpService)
{
	_lookUpService = lookUpService;
	RegisterWithLookUpService("NETO");
	RegisterProperty("NETO", SERVICE1234);
	RegisterProperty("NETO", SERVICE1234_PROXY);
	return;
}

PoolServiceFactory::~PoolServiceFactory()
{ 
	_lookUpService = nullptr;
	_registrationTable.clear(); 
}
PoolResource* PoolServiceFactory::AcquirePoolResource1234()
{
	PoolResource* newResource = new PoolResource();
	return  newResource;
}

PoolResourceProxy* PoolServiceFactory::AcquirePoolResource1234Proxy()
{
	PoolResourceProxy* newProxy = new PoolResourceProxy();
	return newProxy;
}

void PoolServiceFactory::Dispose()
{ 
	//Unregister with all lookup services
	UnRegisterProperty("NETO", SERVICE1234);
	UnRegisterProperty("NETO", SERVICE1234_PROXY);
	UnRegisterWithLookUpService("NETO");
	return;
}

ILookUpServiceBase * PoolServiceFactory::AcquireLookUpServiceAccessPoint(string context)
{
	if (context == "NETO")
		return _lookUpService;  
	else
		return nullptr;
}

__int64 PoolServiceFactory::RegisterWithLookUpService(string context)
{
	__int64 regID;
	if (VerifyRegistration(context) == false)
	{ 
		regID = (AcquireLookUpServiceAccessPoint(context) != nullptr) 
					? AcquireLookUpServiceAccessPoint(context)->RegisterProvider(this) 
					: 0;
		if (regID > 0)
			_registrationTable[context] = regID;
	}

	return 0;
}

bool PoolServiceFactory::UnRegisterWithLookUpService(string context)
{ 
	__int64 regID;
	bool result = false;
	if (VerifyRegistration(context))
	{
		regID = _registrationTable[context];
		result = (AcquireLookUpServiceAccessPoint(context) != nullptr)
					? AcquireLookUpServiceAccessPoint(context)->UnRegisterProvider(regID)
					: false;
		if(result) 
			_registrationTable.erase(context); 
	}
	return result;
}

void PoolServiceFactory::RegisterProperty(string context, string property)
{
	if (VerifyRegistration(context))
	{
		__int64 regID = _registrationTable[context];
		if(AcquireLookUpServiceAccessPoint(context) != nullptr) 
			AcquireLookUpServiceAccessPoint(context)->AddProperty(regID, property);
	}
	return;
}

void PoolServiceFactory::UnRegisterProperty(string context, string property)
{
	if (VerifyRegistration(context))
	{
		__int64 regID = _registrationTable[context];
		if (AcquireLookUpServiceAccessPoint(context) != nullptr)
			AcquireLookUpServiceAccessPoint(context)->RemoveProperty(regID, property);
	}
}

bool PoolServiceFactory::VerifyRegistration(string context)
{
	map<string, __int64>::iterator itr;
	itr = _registrationTable.find(context);
	if (itr != _registrationTable.end())
		return true;
	else
		return false;
}
