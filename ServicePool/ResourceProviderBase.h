#pragma once

#include "LookUpServiceBase.h"
#include <string>
using namespace std;

class ResourceProviderBase 
{
private:
	virtual LookUpServiceBase* AquireLookUpServiceAccessPoint(string context) { return &LookUpServiceBase(); }
	virtual __int64 RegisterWithLookUpService(string context) { return 0; }
	virtual bool UnRegisterWithLookUpService(string context) { return false; }
	virtual void RegisterProperty(string context, string property) {}
	virtual void UnRegisterProperty(string context, string property) {}
};
