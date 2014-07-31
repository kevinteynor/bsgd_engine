#include <Qt\qdebug.h>
#include <Qt\qpainter.h>
#include <Qt\qtimer.h>
#include <QtGui\qboxlayout.h>
#include <QtGui\qlabel.h>
#include <QtGui\qcheckbox.h>
#include <glm\glm.hpp>

#include "DemoWindow.h"
#include "GridWidget2D.h"
#include "UpdateClock.h"
//#include "VerticalDebugMenu.h"

DemoWindow::DemoWindow(){}
DemoWindow::~DemoWindow()
{
	delete debugLayout;
	delete gridDisplay;
	delete mainLayout;
	delete updateTimer;
	delete mainWindow;

	debugLayout = 0;
	gridDisplay = 0;
	mainLayout = 0;
	updateTimer = 0;
	mainWindow = 0;
}

void DemoWindow::Initialize()
{
	mainWindow = new QWidget;
	mainWindow->setLayout(mainLayout = new QVBoxLayout);

	mainLayout->addLayout(debugLayout = new QVBoxLayout);
	//VerticalDebugMenu::Initialize(debugLayout);

	gridDisplay = new GridWidget2D;
	gridDisplay->setMinimumWidth(500);
	gridDisplay->setMinimumHeight(500);
	mainLayout->addWidget(gridDisplay);

	InitializeDebugGui();

	UpdateClock::Initialize();
	updateTimer = new QTimer(this);
	connect(updateTimer, SIGNAL(timeout()), this, SLOT(Update()));
	updateTimer->start();
	mainWindow->show();
}

void DemoWindow::Update()
{
	UpdateClock::Update();
	//VerticalDebugMenu::Update();

	gridDisplay->repaint();
}

PointWatchInfo* DemoWindow::WatchVector(glm::vec2* v, Qt::GlobalColor color)
{
	return gridDisplay->WatchPoint(&v->x, &v->y, NULL, color);
}
PointWatchInfo* DemoWindow::WatchPoint(glm::vec2* v, const float* iMass, Qt::GlobalColor color, bool circle)
{
	return gridDisplay->WatchPoint(&v->x, &v->y, iMass, color, circle);
}
AttachedVectorWatchInfo* DemoWindow::WatchVector(const glm::vec2* v, const glm::vec2* origin, Qt::GlobalColor color)
{
	return gridDisplay->WatchAttachedVector(&v->x, &v->y, &origin->x, &origin->y, color);
}