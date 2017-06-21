#pragma once

#include<string>
using namespace std;

#include "LookUpServiceBase.h"

class LookUpServiceClientBase  
{
private:
	virtual void* AquireResourceProviderByQuery(LookUpServiceBase* service, string property) { return nullptr; }
	virtual LookUpServiceBase* AquireLookUpServiceAccessPoint(string context) { return &LookUpServiceBase(); }
};