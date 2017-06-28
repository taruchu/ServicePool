#pragma once

#include "IPoolResourceContext.h"

class PoolResourceContext : public IPoolResourceContext
{
public:
	int GetParam1() { return _data; }
	void SetParam1(int value) { _data = value; }
	string GetParam2() { return _message; }
	void SetParam2(string value) { _message = value; }
private:
	string _message;
	int _data;
};