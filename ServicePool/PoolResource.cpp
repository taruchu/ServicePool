#include "PoolResource.h"
#include <string>
#include <iostream>
using namespace std;
  
void PoolResource::Service()
{   
	//TODO: Implement another service of some kind, maybe a socket or thread?
	cout << "Jesus Loves You :=)" <<endl;
	return;
}

void PoolResource::InitializeServiceResources()
{
	if(!_isInitialized) 
		//NOTE: Any Initialization overhead goes here.
		_isInitialized = true;
	return;
}

void PoolResource::ReleaseServiceResources()
{
	if(_isInitialized) 
		_isInitialized = false;
	return;
}

void PoolResource::Reset()
{
	if (_isInitialized)
	{
		//Reset the Service object
		ReleaseServiceResources(); 
		InitializeServiceResources();
	}
	return;
}
