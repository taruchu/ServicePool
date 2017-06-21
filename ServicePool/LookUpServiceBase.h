#pragma once
#include <string>
#include <vector> 
using namespace std;



class LookUpServiceBase  
{
public:
	virtual void* FirstResourceProviderMatching(string property) { return nullptr; };
	virtual __int64 RegisterProvider(void* reference) { return 0; }
	virtual bool UnRegisterProvider(__int64 refID) { return false; }
	virtual bool AddProperty(__int64 refID, string property) { return false; }
	virtual bool RemoveProperty(__int64 refID, string property) { return false; }
};