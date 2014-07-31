#ifndef UPDATE_CLOCK_H
#define UPDATE_CLOCK_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "ExportHeader.h"

class ENGINE_SHARED UpdateClock
{
private:
	static LARGE_INTEGER timeFrequency;
	static LARGE_INTEGER lastFrameTime;
	static float deltaTime;

	UpdateClock(void);
	~UpdateClock(void);
	UpdateClock& operator=(const UpdateClock&);

public:
	static bool Initialize();
	static void Update();
	static float DeltaTime();
	static void SetDeltaTime(float dt);
};

#endif // UPDATE_CLOCK_H