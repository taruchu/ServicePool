#pragma once

#include "PoolResource.h"
#include "PoolResourceProxy.h"
#include "IResourceProviderBase.h" 
#include <map>
using namespace std;

class PoolServiceFactory : public IResourceProviderBase
{
public:
	PoolServiceFactory() { _lookUpService = nullptr; }
	PoolServiceFactory(ILookUpServiceBase* lookUpService);
	~PoolServiceFactory();
	
	PoolResource* AcquirePoolResource1234();
	PoolResourceProxy* AcquirePoolResource1234Proxy();
	void Dispose();
	
private:
	ILookUpServiceBase* _lookUpService;
	string const SERVICE1234 = "service1234";
	string const SERVICE1234_PROXY = "service1234_proxy";
	map<string, __int64> _registrationTable;

	ILookUpServiceBase* AcquireLookUpServiceAccessPoint(string context);
	__int64 RegisterWithLookUpService(string context);
	bool UnRegisterWithLookUpService(string congtext);
	void RegisterProperty(string context, string property);
	void UnRegisterProperty(string context, string property); 
	bool VerifyRegistration(string context);
};