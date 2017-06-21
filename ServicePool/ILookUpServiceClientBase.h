#pragma once

#include<string>
using namespace std;

#include "ILookUpServiceBase.h"

class ILookUpServiceClientBase  abstract
{
private:
	virtual void* AcquireResourceProviderByQuery(ILookUpServiceBase* service, string property) = 0;  
	virtual ILookUpServiceBase* AcquireLookUpServiceAccessPoint(string context) = 0;  
};