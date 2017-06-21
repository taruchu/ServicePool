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
	return;
}

void PoolResource::ReleaseServiceResources()
{
	return;
}

void PoolResource::Reset()
{
	//Reset the Service object
	ReleaseServiceResources();
	InitializeServiceResources();
	return;
}
