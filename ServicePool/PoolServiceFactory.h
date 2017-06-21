#pragma once

#include "PoolResource.h"
#include "ResourceProviderBase.h"
#include <map>
using namespace std;

class PoolServiceFactory : public ResourceProviderBase
{
public:
	PoolServiceFactory() { _lookUpService = nullptr; }
	PoolServiceFactory(LookUpServiceBase* lookUpService);
	~PoolServiceFactory();
	
	PoolResource AquirePoolResource1234();
	void Dispose();
	//PoolResource AquirePoolResource4576();
	
private:
	LookUpServiceBase* _lookUpService;
	string const SERVICE1234 = "service1234";
	map<string, __int64> _registrationTable;

	LookUpServiceBase* AquireLookUpServiceAccessPoint(string context);
	__int64 RegisterWithLookUpService(string context);
	bool UnRegisterWithLookUpService(string congtext);
	void RegisterProperty(string context, string property);
	void UnRegisterProperty(string context, string property); 
	bool VerifyRegistration(string context);
};