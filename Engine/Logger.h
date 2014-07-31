#ifndef _LOGGER_H_
#define _LOGGER_H_

#include "ExportHeader.h"
#include "EngineTypeDefs.h"

#ifdef LOGGING

class ENGINE_SHARED Logger
{
private:
	static Logger instance;
	Logger() {}
	~Logger() {}
	Logger(const Logger&);
	Logger& operator=(const Logger&);
public:
	static void Initialize(char* fileName = "log.html");
	static void ShutDown();
	void InternalLog(const char* message, enum SEVERITY sev, const char* fileName, int lineNumber);
	
	static Logger& getInstance() {return instance;}
	static void Log(const char* message, enum SEVERITY sev, const char* fileName, int lineNumber)
	{
		instance.InternalLog(message, sev, fileName, lineNumber);
	}
};

enum SEVERITY { Info = 0, Warning = 1, Error = 2, Severe = 3 } ;

#define LOG(message, severity) Logger::Log(message, severity, __FILE__, __LINE__)
#define LOG_D(message, severity, file, line) Logger::Log(message, severity, file, line)
#endif

#ifndef LOGGING

#define LOG(message, severity)
#define LOG_D(message, severity, file, line)

class ENGINE_SHARED Logger
{
private:
	static Logger instance;
	Logger() {}
public:
	static void Initialize(char* fileName = "log.html") { fileName; }
	static void ShutDown() {}
};

#endif // LOGGING

#endif //_LOGGER_H_