  

#include<iostream>
#include<ctime>
using namespace std;

#include"ResourcePool.h" 
#include "NETOLookUp.h"
#include "PoolServiceFactory.h"
 
int main()
{  
	NETOLookUp* lookup = new NETOLookUp();
	PoolServiceFactory factory(lookup);
	

	ResourcePool pool;
	pool.Initialize(3, lookup);

	if(pool.PoolIsReady())
	{ 
		PoolResourceHandle handle = pool.Aquire();
		if (handle.ValidateHandle())
		{
			handle->Service(); 
			handle.Release();
		}
	}

	factory.Dispose();
	delete lookup;
    return 0;
}

