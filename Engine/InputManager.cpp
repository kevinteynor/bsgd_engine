#include "InputManager.h"

#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>

#include <Qt\qdebug.h>

InputManager* InputManager::localInstance = 0;

InputManager::InputManager(void)
{
	atexit(&CleanUp);
}
void InputManager::CleanUp()
{
	delete localInstance;
	localInstance = 0;
}

InputManager& InputManager::Instance()
{
	if(localInstance == 0)
		localInstance = new InputManager();

	return *localInstance;
}

bool InputManager::IsKeyDown(uint key)
{
	// value is negative if the key is being pressed.
	// value is 0 (maybe positive) if the key is NOT being pressed.
	return GetAsyncKeyState(key) < 0;
}