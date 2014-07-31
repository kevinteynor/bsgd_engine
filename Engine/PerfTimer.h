#ifndef _PERF_TIMER_
#define _PERF_TIMER_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "ExportHeader.h"

namespace Engine
{
	class ENGINE_SHARED PerfTimer
	{
	private:
		LARGE_INTEGER start, end, frequency, lastInterval, nextInterval;

	public:
		PerfTimer(void){lastInterval.QuadPart = 0;}
		~PerfTimer(void){}

		void Start();
		float Stop();

		inline LARGE_INTEGER GetStart();
		inline LARGE_INTEGER GetEnd();
		inline LARGE_INTEGER GetFrequency();
		inline float GetElapsedTime();
		float Interval();
	};
}

#endif // _PERF_TIMER_