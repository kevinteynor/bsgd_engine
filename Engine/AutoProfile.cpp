#include "AutoProfile.h"
#include <fstream>
#include <string>

using Engine::AutoProfile;
using std::fstream;
using std::ofstream;

vector< vector <string> > AutoProfile::log;
vector<string> AutoProfile::currentLineString;
unsigned AutoProfile::currentLine;
unsigned AutoProfile::currentColumn;
unsigned AutoProfile::columnCount;

AutoProfile::AutoProfile()
{
	timer = PerfTimer();
	timer.Start();
}
AutoProfile::~AutoProfile()
{
	char t[30];
	sprintf_s(t, 30, "%0.0f", timer.Stop());
	AddString(t);
}
void AutoProfile::Initialize(vector<string> colHeaders, const unsigned colCount)
{
	columnCount = colCount;
	currentLine = 0;
	currentColumn = 0;
	
	log.push_back(colHeaders);
}
void AutoProfile::AddString(const string& l)
{
	currentLineString.push_back(l);
	currentColumn++;

	if(currentColumn >= columnCount)
	{
		currentLine++;
		log.push_back(currentLineString);
		currentLineString.clear();
		currentColumn = 0;
	}
}
void AutoProfile::DumpLog()
{
	ofstream outfile;
	outfile.open("_log_.csv", fstream::out);

	for(unsigned row = 0; row < log.size(); row++)
	{
		for(unsigned col = 0; col < columnCount; col++)
		{
			if(col != 0)
				outfile << ",";
			outfile << log[row][col];
		}
		outfile << "\n";
	}

	outfile.close();
}