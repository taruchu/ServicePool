#pragma once

#include "ILookUpServiceBase.h"
#include <string>
using namespace std;

class IResourceProviderBase abstract
{
private:
	virtual ILookUpServiceBase* AcquireLookUpServiceAccessPoint(string context) = 0;  
	virtual __int64 RegisterWithLookUpService(string context) = 0;  
	virtual bool UnRegisterWithLookUpService(string context) = 0;  
	virtual void RegisterProperty(string context, string property) = 0;  
	virtual void UnRegisterProperty(string context, string property) = 0;  
};
