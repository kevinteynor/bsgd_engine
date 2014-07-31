#include "Logger.h"
#include <fstream>
#include <cstdio>
#include <ctime>

#pragma warning( disable : 4996)

#ifdef LOGGING
static std::ofstream outFile;
Logger Logger::instance;


void Logger::Initialize(char* fileName)
{
	outFile.open(fileName);

	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer, 80, "%Y-%m-%d-%H-%M-%S", timeinfo);

	outFile << "<html><body><ul><dl>" << std::endl;
	outFile << "<div>" << "Log Date: " << buffer << "<\\div>" << std::endl;
}
void Logger::ShutDown()
{
	outFile << "</dl></ul></body></html>" << std::endl;

	outFile.close();
}
void Logger::InternalLog(const char* message, enum SEVERITY sev, const char* fileName, int lineNumber)
{
	switch(sev)
	{
	case Info:	
		outFile << "<dt style=\"background:white\">" << message << "  (Info)" << std::endl;
		outFile << "\t" << "<dd style=\"background:white\"> File: " << fileName << "</dd>" << std::endl;
		outFile << "\t" << "<dd style=\"background:white\"> Line: " << lineNumber << "</dd></dt>" << std::endl;
		break;
	case Warning:
		outFile << "<dt style=\"background:yellow\">" << message << "  (Warning)" << std::endl;
		outFile << "\t" << "<dd style=\"background:yellow\"> File: " << fileName << "</dd>" << std::endl;
		outFile << "\t" << "<dd style=\"background:yellow\"> Line: " << lineNumber << "</dd></dt>" << std::endl;
		break;
	case Error:
		outFile << "<dt style=\"background:red\">" << message << "  (Error)" << std::endl;
		outFile << "\t" << "<dd style=\"background:red\"> File: " << fileName << "</dd>" << std::endl;
		outFile << "\t" << "<dd style=\"background:red\"> Line: " << lineNumber << "</dd></dt>" << std::endl;
		break;
	case Severe:
		outFile << "<dt style=\"background:red\">" << message << "  (Severe)" << std::endl;
		outFile << "\t" << "<dd style=\"background:red\"> File: " << fileName << "</dd>" << std::endl;
		outFile << "\t" << "<dd style=\"background:red\"> Line: " << lineNumber << "</dd></dt>" << std::endl;
		break;
	}
}
#endif