#pragma once

#include<string>
using namespace std;

class Logger
{
public:
	Logger() {}
	~Logger() {}
	static void LogMessage(string msg, const string file, const int line);
private:
	static const string _LOGFILE_PATH; 
};