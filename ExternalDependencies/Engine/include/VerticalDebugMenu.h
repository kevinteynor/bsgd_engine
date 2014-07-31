#ifndef VERTICAL_DEBUG_MENUS_H
#define VERTICAL_DEBUG_MENUS_H

#include <Qt\qobject.h>
#include <glm\glm.hpp>
#include "ExportHeader.h"
#include "FastDelegate.h"
#include "FastDelegateBind.h"
#include "EngineTypeDefs.h"

#ifdef USE_VERTICAL_DEBUG_MENUS

class QBoxLayout;
class QVBoxLayout;
class QLabel;
class QCheckBox;
class QPushButton;
namespace Neumont { class Slider; }

struct FloatWatchInfo;
struct FloatWatchSliderInfo;
struct BoolWatcherInfo;
struct Vec2WatchInfo;
struct ButtonWatchInfo;

class ENGINE_SHARED VerticalDebugMenu
{
private:
	static QBoxLayout* containerLayout;
	static QVBoxLayout* debugLayout;

	static const uint MAX_WATCHED_VALUES = 24;
	static uint totalWatchedCount;

	static uint nextFloatWatcher;
	static FloatWatchInfo floatWatchInfos[MAX_WATCHED_VALUES];

	static uint nextFloatSliderWatcher;
	static FloatWatchSliderInfo floatWatchSliderInfos[MAX_WATCHED_VALUES];

	static uint nextBoolWatcher;
	static BoolWatcherInfo boolWatcherInfos[MAX_WATCHED_VALUES];

	static uint nextVec2Watcher;
	static Vec2WatchInfo vec2WatchInfos[MAX_WATCHED_VALUES];

	static uint nextButtonWatcher;
	static ButtonWatchInfo buttonWatchInfos[MAX_WATCHED_VALUES];

	VerticalDebugMenu(){}
	~VerticalDebugMenu();
	VerticalDebugMenu(const VerticalDebugMenu&);
	VerticalDebugMenu& operator=(const VerticalDebugMenu&);

public:
	static void Initialize(QBoxLayout* container);
	static void Shutdown();
	static void Update();

	static FloatWatchInfo* WatchFloat(const char* description, const float* thefloat);
	static FloatWatchSliderInfo* WatchFloatSlider(const char* description, float* thefloat, float min = -10, float max = 10);
	static BoolWatcherInfo* WatchBool(const char* description, bool* thebool);
	static Vec2WatchInfo* WatchVec2(const char* description, glm::vec2* theVec2);
	static ButtonWatchInfo* WatchButton(const char* text, Delegate function);
};

#pragma region Watch Info Definitions
struct FloatWatchInfo
{
	const float* theFloat;
	QLabel* theLabel;

	~FloatWatchInfo(){ theFloat = 0; theLabel = 0; }

};

struct FloatWatchSliderInfo
{
	float* theFloat;
	Neumont::Slider* theSlider;
	float min, max;
	
	~FloatWatchSliderInfo(){ theFloat = 0; theSlider = 0; }

};

struct BoolWatcherInfo
{
	bool* theBool;
	QCheckBox* theCheckbox;

	~BoolWatcherInfo(){ theBool = 0; theCheckbox = 0; }

};

struct Vec2WatchInfo
{
	glm::vec2* theVec2;
	QLabel* theLabel;

	~Vec2WatchInfo(){ theVec2 = 0; theLabel = 0; }

};

struct ButtonWatchInfo : QObject
{
	Q_OBJECT

public:
	Delegate buttonFunction;
	QPushButton* theButton;

	void ConnectButton();
	~ButtonWatchInfo() { theButton = 0; }

public slots:
	void functioncall();
};
#pragma endregion

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
	static void WatchFloatSlider(const char* description, float* thefloat, float min = -10, float max = 10){}
	static void WatchBool(const char* description, bool* thebool){}
	static void WatchVec2(const char* description, glm::vec2* theVec2){}
	static void WatchButton(const char* text, Delegate function){}
};

struct ButtonWatchInfo{};

#endif // USE_DEBUG_MENUS


#endif // VERTICAL_DEBUG_MENUS_H