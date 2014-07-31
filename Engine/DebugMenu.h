#ifndef DEBUG_MENUS_H
#define DEBUG_MENUS_H

#include "ExportHeader.h"
#include "TypeDefs.h"

namespace Neumont { class Slider; }
class QVBoxLayout;
class QHBoxLayout;
class QLabel;
class QCheckBox;

#ifdef USE_DEBUG_MENUS

class ENGINE_SHARED DebugMenu
{
private:
	static QHBoxLayout* mainDebugLayout;
	static QVBoxLayout* columnLayouts[3];

	static const uint MAX_WATCHED_VALUES = 24;
	static uint totalWatchedCount;
	static uint nextColumnIndex;

	static uint nextFloatWatcher;
	static struct FloatWatchInfo
	{
		const float* theFloat;
		QLabel* theLabel;

		~FloatWatchInfo(){ theFloat = 0; theLabel = 0; }

	} floatWatchInfos[MAX_WATCHED_VALUES];

	static uint nextFloatSliderWatcher;
	static struct FloatWatchSliderInfo
	{
		float* theFloat;
		Neumont::Slider* theSlider;

		~FloatWatchSliderInfo(){ theFloat = 0; theSlider = 0; }

	} floatWatchSliderInfos[MAX_WATCHED_VALUES];

	static uint nextBoolWatcher;
	static struct BoolWatcherInfo
	{
		bool* theBool;
		QCheckBox* theCheckbox;

		~BoolWatcherInfo(){ theBool = 0; theCheckbox = 0; }

	} boolWatcherInfos[MAX_WATCHED_VALUES];

	DebugMenu(){}
	~DebugMenu();
	DebugMenu(const DebugMenu&);
	DebugMenu& operator=(const DebugMenu&);

public:
	static void Initialize(QHBoxLayout* debugLayout);
	static void Shutdown();
	static void Update();

	static void WatchFloat(const char* description, const float* thefloat);
	static void WatchFloatSlider(const char* description, float* thefloat);
	static void WatchBool(const char* description, bool* thebool);
};

#else

#pragma warning( disable : 4100)
class ENGINE_SHARED DebugMenu
{
private:
	DebugMenu(){}
	~DebugMenu(){}
	DebugMenu(const DebugMenu&);
	DebugMenu& operator=(const DebugMenu&);

public:
	static void Initialize(QHBoxLayout* debugLayout){}
	static void Shutdown(){}
	static void Update(){}

	static void WatchFloat(const char* description, const float* thefloat){}
	static void WatchFloatSlider(const char* description, float* thefloat){}
	static void WatchBool(const char* description, bool* thebool){}
};

#endif // USE_DEBUG_MENUS


#endif // DEBUG_MENUS_H