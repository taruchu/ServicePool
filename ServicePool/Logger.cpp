#include "Logger.h"
#include<fstream>
#include<ctime>

const string Logger::_LOGFILE_PATH = "Log.txt";

void Logger::LogMessage(string msg, const string file, const int line)
{
	time_t _tm = time(NULL);
	struct tm * curtime = localtime(&_tm); 

	fstream log;
	log.open(_LOGFILE_PATH, fstream::out | fstream::app);
	if (log.is_open())
	{
		log << asctime(curtime) << endl << " file: " << file << endl << " line: " << line << endl << " message: " << msg << endl << endl;
		log.close();
	}	
	return;
}
