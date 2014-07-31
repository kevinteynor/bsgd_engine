#ifndef _AUTO_PROFILE_H_
#define _AUTO_PROFILE_H_

#include <vector>
#include "ExportHeader.h"
#include "PerfTimer.h"

using std::vector;
using std::string;
using Engine::PerfTimer;

namespace Engine
{
	class ENGINE_SHARED AutoProfile
	{

	private:
		static vector< vector <string> > log;
		static vector<string> currentLineString;
		static unsigned currentLine;
		static unsigned currentColumn;
		static unsigned columnCount;

		PerfTimer timer;

	public:
		AutoProfile(void);
		~AutoProfile(void);

		static void AddString(const string& l);	//{ log[0][0] = l; }
		static void Initialize(vector<string> colHeaders, const unsigned colCount);
		static void DumpLog();
	};
}
#endif //_AUTO_PROFILE_H_