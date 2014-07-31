#include "VerticalDebugMenu.h"

#ifdef USE_VERTICAL_DEBUG_MENUS

#include <Qt\qdebug.h>
#include <Qt\qpushbutton.h>
#include <QtGui\qcheckbox.h>
#include <QtGui\qhboxlayout>
#include <QtGui\qlabel.h>
#include <Slider.h>
#include "InputManager.h"

uint VerticalDebugMenu::totalWatchedCount = 0;
uint VerticalDebugMenu::nextFloatWatcher = 0;
uint VerticalDebugMenu::nextFloatSliderWatcher = 0;
uint VerticalDebugMenu::nextBoolWatcher = 0;
uint VerticalDebugMenu::nextVec2Watcher = 0;
uint VerticalDebugMenu::nextButtonWatcher = 0;
QBoxLayout* VerticalDebugMenu::containerLayout;
QVBoxLayout* VerticalDebugMenu::debugLayout;

FloatWatchInfo VerticalDebugMenu::floatWatchInfos[VerticalDebugMenu::MAX_WATCHED_VALUES];
FloatWatchSliderInfo VerticalDebugMenu::floatWatchSliderInfos[VerticalDebugMenu::MAX_WATCHED_VALUES];
BoolWatcherInfo VerticalDebugMenu::boolWatcherInfos[VerticalDebugMenu::MAX_WATCHED_VALUES];
Vec2WatchInfo VerticalDebugMenu::vec2WatchInfos[VerticalDebugMenu::MAX_WATCHED_VALUES];
ButtonWatchInfo VerticalDebugMenu::buttonWatchInfos[VerticalDebugMenu::MAX_WATCHED_VALUES];

VerticalDebugMenu::~VerticalDebugMenu(void)
{
	Shutdown();
}

void VerticalDebugMenu::Initialize(QBoxLayout* container)
{
	containerLayout = container;

	containerLayout->setContentsMargins(0,0,0,0);
	
	// QHBOXLAYOUT
	containerLayout->addLayout(debugLayout = new QVBoxLayout);
}
void VerticalDebugMenu::Shutdown()
{
	containerLayout = 0;
	debugLayout = 0;

#pragma warning( disable : 4154)
	delete [] floatWatchInfos;
	delete [] floatWatchSliderInfos;
	delete [] boolWatcherInfos;
	delete [] vec2WatchInfos;
}

FloatWatchInfo* VerticalDebugMenu::WatchFloat(const char* description, const float* thefloat)
{
	if(totalWatchedCount >= MAX_WATCHED_VALUES)
		return 0;

	QHBoxLayout* thisLayout;

	debugLayout->addLayout(thisLayout = new QHBoxLayout);

	thisLayout->addWidget(new QLabel(description));
	FloatWatchInfo& info = floatWatchInfos[nextFloatWatcher++];
	thisLayout->addWidget(info.theLabel = new QLabel());
	info.theLabel->setMaximumHeight(60);
	info.theFloat = thefloat;

	totalWatchedCount++;
	return &info;
}
FloatWatchSliderInfo* VerticalDebugMenu::WatchFloatSlider(const char* description, float* thefloat, float min, float max)
{
	if(totalWatchedCount >= MAX_WATCHED_VALUES)
		return 0;

	QHBoxLayout* thisLayout;

	debugLayout->addLayout(thisLayout = new QHBoxLayout);

	thisLayout->addWidget(new QLabel(description));

	FloatWatchSliderInfo& info = floatWatchSliderInfos[nextFloatSliderWatcher++];
	thisLayout->addWidget(info.theSlider = new Neumont::Slider(min, max));
	info.theSlider->setMaximumHeight(60);
	info.theFloat = thefloat;

	info.theSlider->setValue(*thefloat);

	totalWatchedCount++;
	return &info;
}
BoolWatcherInfo* VerticalDebugMenu::WatchBool(const char* description, bool* thebool)
{
	if(totalWatchedCount >= MAX_WATCHED_VALUES)
		return 0;

	QHBoxLayout* thisLayout;

	debugLayout->addLayout(thisLayout = new QHBoxLayout);

	thisLayout->addWidget(new QLabel(description));

	BoolWatcherInfo& info = boolWatcherInfos[nextBoolWatcher++];
	thisLayout->addWidget(info.theCheckbox = new QCheckBox);
	info.theCheckbox->setMaximumHeight(60);
	info.theBool = thebool;

	info.theCheckbox->setChecked(*thebool);

	totalWatchedCount++;
	return &info;
}
Vec2WatchInfo* VerticalDebugMenu::WatchVec2(const char* description, glm::vec2* theVec2)
{
	if(totalWatchedCount >= MAX_WATCHED_VALUES)
		return 0;

	QHBoxLayout* thisLayout;

	debugLayout->addLayout(thisLayout = new QHBoxLayout);

	thisLayout->addWidget(new QLabel(description));

	Vec2WatchInfo& info = vec2WatchInfos[nextVec2Watcher++];
	thisLayout->addWidget(info.theLabel = new QLabel());
	info.theLabel->setMaximumHeight(60);
	info.theVec2 = theVec2;

	totalWatchedCount++;
	return &info;
}
ButtonWatchInfo* VerticalDebugMenu::WatchButton(const char* name, Delegate function)
{
	if(totalWatchedCount >= MAX_WATCHED_VALUES)
		return 0;

	QHBoxLayout* thisLayout;

	debugLayout->addLayout(thisLayout = new QHBoxLayout);

	ButtonWatchInfo& info = buttonWatchInfos[nextButtonWatcher++];
	thisLayout->addWidget(info.theButton = new QPushButton(name));
	info.ConnectButton();
	info.buttonFunction = function;

	totalWatchedCount++;
	return &info;
}

void VerticalDebugMenu::Update()
{
	if(containerLayout == 0)
		return;

	for(uint i = 0; i < nextFloatWatcher; i++)
	{
		const FloatWatchInfo& info = floatWatchInfos[i];
		QString text;
		text.setNum(*(info.theFloat));
		info.theLabel->setText(text);
	}
	for(uint i = 0; i < nextFloatSliderWatcher; i++)
	{
		const FloatWatchSliderInfo& info = floatWatchSliderInfos[i];
		*(info.theFloat) = info.theSlider->value();
	}
	for(uint i = 0; i < nextBoolWatcher; i++)
	{
		const BoolWatcherInfo& info = boolWatcherInfos[i];
		*(info.theBool) = (info.theCheckbox->checkState() == 2);	// 2 = Qt::CheckedState::Checked
	}
	for(uint i = 0; i < nextVec2Watcher; i++)
	{
		const Vec2WatchInfo& info = vec2WatchInfos[i];
		QString text = "< " + QString::number(info.theVec2->x) + " , " + QString::number(info.theVec2->y) + " >";
		info.theLabel->setText(text);
	}
}

void ButtonWatchInfo::ConnectButton()
{
	connect(theButton, SIGNAL(released()), this, SLOT(functioncall()));
}
void ButtonWatchInfo::functioncall()
{
	buttonFunction();
}

#endif // USE_DEBUG_MENUS