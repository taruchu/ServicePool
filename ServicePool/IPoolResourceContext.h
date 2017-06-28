#pragma once
#include <string>
using namespace std;
 
class IPoolResourceContext abstract
{
public:
	virtual int GetParam1() = 0;
	virtual void SetParam1(int value) = 0;
	virtual string GetParam2() = 0;
	virtual void SetParam2(string value) = 0;
};
