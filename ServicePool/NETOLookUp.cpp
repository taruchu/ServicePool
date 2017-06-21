#include "NETOLookUp.h"

void* NETOLookUp::FirstResourceProviderMatching(string property)
{
	if(VerifyProperty(property) == false)
		return nullptr;
	else
	{ 
		map<__int64, void*>::iterator itr;
		__int64 index;
		for (itr = _propertyTable[property].begin(); itr != _propertyTable[property].end();)
		{
			index = itr->first;
			if (VerifyReferenceID(index))
			{ 
				return _resourceProviders[index];
			}
			else
				++itr;
		}
		return nullptr;
	}
}

__int64 NETOLookUp::RegisterProvider(void * reference)
{
	__int64 index = GetNewIndex(); 
	_resourceProviders[index] = reference; 
	return index;
}

bool NETOLookUp::UnRegisterProvider(__int64 refID)
{
	if (VerifyReferenceID(refID) == false)
		return false;
	else
	{  
		CleanPropertyTable(refID);
		_resourceProviders.erase(refID); 		
	} 
	return true;
}

bool NETOLookUp::AddProperty(__int64 refID, string property)
{
	if (VerifyReferenceID(refID))
	{
		_propertyTable[property][refID] = nullptr;
		return true;
	}
	else
		return false;
}

bool NETOLookUp::RemoveProperty(__int64 refID, string property)
{
	if (VerifyReferenceID(refID) && VerifyProperty(property))
	{
		_propertyTable[property].erase(refID);
		if (_propertyTable[property].size() == 0)
			_propertyTable.erase(property);
		return true;
	}
	return false;
}

__int64 NETOLookUp::GetNewIndex()
{
	return  std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count(); 
}

bool NETOLookUp::VerifyReferenceID(__int64 refID)
{
	map<__int64, void*>::iterator itr;
	itr = _resourceProviders.find(refID);
	return (itr != _resourceProviders.end()); 
}
 
bool NETOLookUp::VerifyProperty(string property)
{
	map<string, map<__int64, void*>>::iterator itr;
	itr = _propertyTable.find(property);
	return (itr != _propertyTable.end());
}

void NETOLookUp::CleanPropertyTable(__int64 refID)
{
	map<string, map<__int64, void*>>::iterator itr;
	for (itr = _propertyTable.begin(); itr != _propertyTable.end();)
	{
		itr->second.erase(refID);
		if (itr->second.size() == 0)
			_propertyTable.erase(itr++);
		else
			++itr;
	}
	return;
}

 
 
