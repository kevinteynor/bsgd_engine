#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "ExportHeader.h"
#include "TypeDefs.h"

class ENGINE_SHARED InputManager
{
private:
	InputManager(void);
	~InputManager(void){}
	InputManager& operator=(const InputManager&);
	static void CleanUp();

	static InputManager* localInstance;

public:
	static InputManager& Instance();

	static bool IsKeyDown(uint key);
};


#endif // INPUT_MANAGER_H