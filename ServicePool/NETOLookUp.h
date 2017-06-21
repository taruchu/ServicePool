#pragma once

#include "LookUpServiceBase.h"
#include <chrono>
#include <map>
#include <vector>
#include <string> 

using namespace std; 
 

class NETOLookUp : public LookUpServiceBase
{
public:
	NETOLookUp() {} 
	~NETOLookUp()
	{
		_resourceProviders.clear();  
		_propertyTable.clear();
	}
	void* FirstResourceProviderMatching(string property);
	__int64 RegisterProvider(void* reference);
	bool UnRegisterProvider(__int64 refID);
	bool AddProperty(__int64 refID, string property);
	bool RemoveProperty(__int64 refID, string property);
private:  
	map<__int64, void*> _resourceProviders; 
	map<string, map<__int64, void*>> _propertyTable; 
	__int64 GetNewIndex();
	bool VerifyReferenceID(__int64 refID);
	bool VerifyProperty(string property);   
	void CleanPropertyTable(__int64 refID);
};