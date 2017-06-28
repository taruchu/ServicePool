  

#include<iostream>
#include<ctime>
using namespace std;

#include"ResourcePool.h" 
#include "NETOLookUp.h"
#include "PoolServiceFactory.h"
#include "PoolResourceContext.h"
 
int main()
{  
	NETOLookUp* lookup = new NETOLookUp();
	PoolServiceFactory factory(lookup);
	

	ResourcePool pool;
	pool.Initialize(20, lookup, 5, false); //NOTE: Since the lazy aquizition flag is false, the "emergency lazy aquisistion" will 
										//kick in once the pool contains more than 5 allocated resources. This is because I used 5 for the 
										//eager aquisition limit. Five is less than the max of 20, so the emergency system will 
										//allocate 15 more using a proxy instead of the real resource. But it has to wait until
										//all 5 eager allocations are used up.
	if(pool.PoolIsReady())
	{  
		PoolResourceHandle handle1 = pool.AcquireResource();
		handle1.GetNewLease(100);
		PoolResourceContext *context = new PoolResourceContext;
		context->SetParam2("Love is all you need.");

		while(handle1.CheckLease()) 
		{
			handle1.Resource()->Service(context);
		} 		 
		if(!handle1.CheckLease()) 
			handle1.Release();

		handle1 = pool.AcquireResource();
		handle1.GetNewLease(100);
		while (handle1.CheckLease())
		{
			handle1.Resource()->Service(context);
			handle1.Release();
		}
		 
		delete context;
	}

	factory.Dispose();
	delete lookup;
    return 0;
}

