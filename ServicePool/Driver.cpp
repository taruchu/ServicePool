  

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
	pool.Initialize(20, lookup, 5, false);

	if(pool.PoolIsReady())
	{ 
		PoolResourceHandle handle1 = pool.Acquire();
		PoolResourceHandle handle2 = pool.Acquire();
		PoolResourceHandle handle3 = pool.Acquire();
		PoolResourceHandle handle4 = pool.Acquire();
		PoolResourceHandle handle5 = pool.Acquire();
		PoolResourceHandle handle6 = pool.Acquire();//NOTE: Proxy should "kick in" here.

		if (handle1.ValidateHandle())
		{
			handle1->Service(); 
			handle1.Release();
		}
		if (handle2.ValidateHandle())
		{
			handle2->Service();
			handle2.Release();
		}
		if (handle3.ValidateHandle())
		{
			handle3->Service();
			handle3.Release();
		}
		if (handle4.ValidateHandle())
		{
			handle4->Service();
			handle4.Release();
		}
		if (handle5.ValidateHandle())
		{
			handle5->Service();
			handle5.Release();
		}
		if (handle6.ValidateHandle())
		{
			handle6->Service();
			handle6.Release();
		}
	}

	factory.Dispose();
	delete lookup;
    return 0;
}

