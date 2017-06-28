#pragma once

class ILeaseGrantor;

class ILeaseHolder abstract
{
public: 
	virtual bool CheckLease() = 0;	
};