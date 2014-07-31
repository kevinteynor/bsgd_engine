#include "DebugMenu.h"

#ifdef USE_DEBUG_MENUS

#include <Qt\qdebug.h>
#include <QtGui\qcheckbox.h>
#include <QtGui\qhboxlayout>
#include <QtGui\qlabel.h>
#include <Slider.h>
#include "InputManager.h"

uint DebugMenu::totalWatchedCount = 0;
uint DebugMenu::nextColumnIndex = 0;
uint DebugMenu::nextFloatWatcher = 0;
uint DebugMenu::nextFloatSliderWatcher = 0;
uint DebugMenu::nextBoolWatcher = 0;
QHBoxLayout* DebugMenu::mainDebugLayout;
QVBoxLayout* DebugMenu::columnLayouts[3];

DebugMenu::FloatWatchInfo DebugMenu::floatWatchInfos[DebugMenu::MAX_WATCHED_VALUES];
DebugMenu::FloatWatchSliderInfo DebugMenu::floatWatchSliderInfos[DebugMenu::MAX_WATCHED_VALUES];
DebugMenu::BoolWatcherInfo DebugMenu::boolWatcherInfos[DebugMenu::MAX_WATCHED_VALUES];

DebugMenu::~DebugMenu(void)
{
	Shutdown();
}

void DebugMenu::Initialize(QHBoxLayout* debugLayout)
{
	mainDebugLayout = debugLayout;

	mainDebugLayout->setContentsMargins(0,0,0,0);
	
	// QHBOXLAYOUT
	mainDebugLayout->addLayout(columnLayouts[0] = new QVBoxLayout);
	mainDebugLayout->addLayout(columnLayouts[1] = new QVBoxLayout);
	mainDebugLayout->addLayout(columnLayouts[2] = new QVBoxLayout);
}
void DebugMenu::Shutdown()
{
	mainDebugLayout = 0;
	columnLayouts[0] = 0;
	columnLayouts[1] = 0;
	columnLayouts[2] = 0;

#pragma warning( disable : 4154)
	delete [] floatWatchInfos;
	delete [] floatWatchSliderInfos;
	delete [] boolWatcherInfos;
}

void DebugMenu::WatchFloat(const char* description, const float* thefloat)
{
	if(totalWatchedCount >= MAX_WATCHED_VALUES)
		return;

	QHBoxLayout* thisLayout;

	columnLayouts[nextColumnIndex]->addLayout(thisLayout = new QHBoxLayout);
	nextColumnIndex += nextColumnIndex < 2 ? 1 : -2;

	thisLayout->addWidget(new QLabel(description));
	FloatWatchInfo& info = floatWatchInfos[nextFloatWatcher++];
	thisLayout->addWidget(info.theLabel = new QLabel());
	info.theLabel->setMaximumHeight(60);
	info.theFloat = thefloat;

	totalWatchedCount++;
}
void DebugMenu::WatchFloatSlider(const char* description, float* thefloat)
{
	if(totalWatchedCount >= MAX_WATCHED_VALUES)
		return;

	QHBoxLayout* thisLayout;

	columnLayouts[nextColumnIndex]->addLayout(thisLayout = new QHBoxLayout);
	nextColumnIndex += nextColumnIndex < 2 ? 1 : -2;

	thisLayout->addWidget(new QLabel(description));

	FloatWatchSliderInfo& info = floatWatchSliderInfos[nextFloatSliderWatcher++];
	thisLayout->addWidget(info.theSlider = new Neumont::Slider);
	info.theSlider->setMaximumHeight(60);
	info.theFloat = thefloat;

	info.theSlider->setValue(*thefloat);

	totalWatchedCount++;
}
void DebugMenu::WatchBool(const char* description, bool* thebool)
{
	if(totalWatchedCount >= MAX_WATCHED_VALUES)
		return;

	QHBoxLayout* thisLayout;

	columnLayouts[nextColumnIndex]->addLayout(thisLayout = new QHBoxLayout);
	nextColumnIndex += nextColumnIndex < 2 ? 1 : -2;

	thisLayout->addWidget(new QLabel(description));

	BoolWatcherInfo& info = boolWatcherInfos[nextBoolWatcher++];
	thisLayout->addWidget(info.theCheckbox = new QCheckBox);
	info.theCheckbox->setMaximumHeight(60);
	info.theBool = thebool;

	info.theCheckbox->setChecked(*thebool);

	totalWatchedCount++;
}

void DebugMenu::Update()
{
	if(mainDebugLayout == 0)
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
}

#endif // USE_DEBUG_MENUS