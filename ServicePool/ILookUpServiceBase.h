#pragma once
#include <string>
#include <vector> 
using namespace std;



class ILookUpServiceBase abstract
{
public:
	virtual void* FirstResourceProviderMatching(string property) = 0; 
	virtual __int64 RegisterProvider(void* reference) = 0;  
	virtual bool UnRegisterProvider(__int64 refID) = 0;  
	virtual bool AddProperty(__int64 refID, string property) = 0;  
	virtual bool RemoveProperty(__int64 refID, string property) = 0;  
};