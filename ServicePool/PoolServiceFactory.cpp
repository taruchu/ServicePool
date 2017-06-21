#include "PoolServiceFactory.h"

PoolServiceFactory::PoolServiceFactory(LookUpServiceBase* lookUpService)
{
	_lookUpService = lookUpService;
	RegisterWithLookUpService("NETO");
	RegisterProperty("NETO", SERVICE1234);
	return;
}

PoolServiceFactory::~PoolServiceFactory()
{ 
	_lookUpService = nullptr;
	_registrationTable.clear(); 
}
PoolResource PoolServiceFactory::AquirePoolResource1234()
{
	PoolResource newResource;
	return  newResource;
}

void PoolServiceFactory::Dispose()
{ 
	//Unregister with all lookup services
	UnRegisterProperty("NETO", SERVICE1234);
	UnRegisterWithLookUpService("NETO");
	return;
}

LookUpServiceBase * PoolServiceFactory::AquireLookUpServiceAccessPoint(string context)
{
	if (context == "NETO")
		return _lookUpService; //NOTE: Would use RPC to get a pointer to this lookup service, keeping it simple for now.
	else
		return nullptr;
}

__int64 PoolServiceFactory::RegisterWithLookUpService(string context)
{
	__int64 regID;
	if (VerifyRegistration(context) == false)
	{ 
		regID = (AquireLookUpServiceAccessPoint(context) != nullptr) 
					? AquireLookUpServiceAccessPoint(context)->RegisterProvider(this) 
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
		result = (AquireLookUpServiceAccessPoint(context) != nullptr)
					? AquireLookUpServiceAccessPoint(context)->UnRegisterProvider(regID)
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
		if(AquireLookUpServiceAccessPoint(context) != nullptr) 
			AquireLookUpServiceAccessPoint(context)->AddProperty(regID, property);
	}
	return;
}

void PoolServiceFactory::UnRegisterProperty(string context, string property)
{
	if (VerifyRegistration(context))
	{
		__int64 regID = _registrationTable[context];
		if (AquireLookUpServiceAccessPoint(context) != nullptr)
			AquireLookUpServiceAccessPoint(context)->RemoveProperty(regID, property);
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
