#include "UpdateClock.h"

float UpdateClock::deltaTime;
LARGE_INTEGER UpdateClock::timeFrequency;
LARGE_INTEGER UpdateClock::lastFrameTime;

bool UpdateClock::Initialize()
{
	int b = QueryPerformanceFrequency(&timeFrequency);
	if(!b)
		return false;
	b = QueryPerformanceCounter(&lastFrameTime);
	if(!b)
		return false;

	return true;
}
void UpdateClock::Update()
{
	LARGE_INTEGER currentFrameTime;
	QueryPerformanceCounter(&currentFrameTime);
	LARGE_INTEGER delta;
	delta.QuadPart = currentFrameTime.QuadPart - lastFrameTime.QuadPart;
	deltaTime = ((float)delta.QuadPart) / ((float)timeFrequency.QuadPart);

	lastFrameTime.QuadPart = currentFrameTime.QuadPart;
}
float UpdateClock::DeltaTime()
{
	return deltaTime;
}
void UpdateClock::SetDeltaTime(float dt)
{
	deltaTime = dt;
}