#pragma once

 

class PoolResource  
{
public: 
	virtual void Service();  
	PoolResource() {}
	~PoolResource()
	{
		ReleaseServiceResources();
	}
private:
	void InitializeServiceResources();
	void ReleaseServiceResources();
	unsigned _refCounter;
	unsigned _handleID;
	virtual void Reset();
	
	friend class PoolResourceHandle;	 
	friend class ResourcePool;
};
