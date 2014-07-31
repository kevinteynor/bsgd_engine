#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "PerfTimer.h"

using Engine::PerfTimer;

void PerfTimer::Start()
{
	QueryPerformanceCounter(&start);
	QueryPerformanceFrequency(&frequency);

	lastInterval.QuadPart = 0;
	nextInterval.QuadPart = 0;
}
float PerfTimer::Stop()
{
	QueryPerformanceCounter(&end);
	return (float)(end.QuadPart - start.QuadPart);
}

inline LARGE_INTEGER PerfTimer::GetStart()
{
	return PerfTimer::start;
}
inline LARGE_INTEGER PerfTimer::GetEnd()
{
	return PerfTimer::end;
}
inline LARGE_INTEGER PerfTimer::GetFrequency()
{
	return PerfTimer::frequency;
}
inline float PerfTimer::GetElapsedTime()
{
	return (float)(end.QuadPart - start.QuadPart);
}
float PerfTimer::Interval()
{
	lastInterval = nextInterval;
	QueryPerformanceCounter(&nextInterval);

	float next = (float)nextInterval.QuadPart / (float)frequency.QuadPart;
	float last = (float)lastInterval.QuadPart / (float)frequency.QuadPart;
	return next - last;
}