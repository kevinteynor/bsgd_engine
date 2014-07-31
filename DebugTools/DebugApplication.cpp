#include "DebugApplication.h"

#include <glm\glm.hpp>
#include <Qt\qcheckbox.h>
#include <Qt\qdockwidget.h>
#include <Qt\qboxlayout.h>
#include <Qt\qlabel.h>
#include <Qt\qmainwindow.h>
#include <Qt\qpushbutton.h>
#include <Qt\qstring.h>
#include <Qt\qvector.h>
#include <Renderer.h>
#include <ShapeData.h>
#include <Slider.h>
#include <UpdateClock.h>
#include "DebugShapes.h"
#include "ExportHeader.h"

#include <Qt\qdebug.h>

QDockWidget* DebugTools::DebugWidget::activeDockWidget;
QWidget* DebugTools::DebugWidget::activeDebugWidget;
QBoxLayout* DebugTools::DebugWidget::mainLayout;
QVector<DebugTools::FloatWatchInfo> DebugTools::DebugWidget::floatWatchInfos;
QVector<DebugTools::FloatWatchSliderInfo> DebugTools::DebugWidget::floatWatchSliderInfos;
QVector<DebugTools::BoolWatcherInfo> DebugTools::DebugWidget::boolWatchInfos;
QVector<DebugTools::Vec2WatchInfo> DebugTools::DebugWidget::vec2WatchInfos;
QVector<DebugTools::Vec3WatchInfo> DebugTools::DebugWidget::vec3WatchInfos;
QVector<DebugTools::ButtonWatchInfo*> DebugTools::DebugWidget::buttonWatchInfos;

Renderer* DebugTools::DebugWidget::ActiveRenderer;

void DebugTools::DebugApplication::Initialize()
{
	// DebugMenu
	debugWidget = new QDockWidget;	
	addDockWidget(Qt::LeftDockWidgetArea, debugWidget);
	DebugTools::DebugWidget::SetActiveDebugDockWidget(debugWidget);

	// Scene + renderer
	setCentralWidget(renderer = new Renderer());
	connect(renderer, SIGNAL(rendererInitialized()), this, SLOT(RendererInitialized()));
	renderer->VERTEX_SIZE = sizeof(Neumont::Vertex);
	renderer->INDEX_SIZE = sizeof(ushort);
	renderer->POSITION_OFFSET = Neumont::Vertex::POSITION_OFFSET;
	renderer->COLOR_OFFSET = Neumont::Vertex::COLOR_OFFSET;
	renderer->NORMAL_OFFSET = Neumont::Vertex::NORMAL_OFFSET;
	renderer->UV_OFFSET = Neumont::Vertex::UV_OFFSET;
	renderer->NORMAL_TANGENT_OFFSET = Neumont::Vertex::TANGENT_OFFSET;
	renderer->NORMAL_BITANGENT_OFFSET = Neumont::Vertex::BITANGENT_OFFSET;
	renderer->show();

	DebugWidget::ActiveRenderer = renderer;

	DebugShapes::SetRenderer(renderer);
	connect(debugWidget, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), this, SLOT(DockWidgetOrientationChanged(Qt::DockWidgetArea)));
	
	UpdateClock::Initialize();
}
void DebugTools::DebugApplication::RendererInitialized()
{
	connect(renderer, SIGNAL(MousePressed(glm::vec2)), this, SLOT(MousePressed(glm::vec2)));
	connect(renderer, SIGNAL(MouseMoved(glm::vec2)), this, SLOT(MouseMoved(glm::vec2)));
	connect(renderer, SIGNAL(preDraw()), this, SLOT(Update()));
}
void DebugTools::DebugApplication::Update()
{
	DebugTools::DebugWidget::UpdateWatchInfos();
}

void DebugTools::DebugApplication::MousePressed(glm::vec2 mousepos){ renderer->setFocus(); }
void DebugTools::DebugApplication::MouseMoved(glm::vec2 mousepos){}
void DebugTools::DebugApplication::DockWidgetOrientationChanged(Qt::DockWidgetArea area)
{
	DebugTools::DebugWidget::ToggleLayoutOrientation(
		(area == Qt::LeftDockWidgetArea || area == Qt::RightDockWidgetArea) ? 0 : 1);
}

void DebugTools::DebugWidget::SetActiveDebugDockWidget(QDockWidget* widget)
{
	activeDockWidget = widget;
	activeDockWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	activeDockWidget->setWidget(activeDebugWidget = new QWidget());
	activeDebugWidget->setLayout(mainLayout = new QBoxLayout(QBoxLayout::TopToBottom));
	mainLayout->setAlignment(Qt::AlignCenter | Qt::AlignTop);
}
void DebugTools::DebugWidget::UpdateWatchInfos()
{
	for(int i = 0; i < floatWatchInfos.size(); i++)	// float
	{
		floatWatchInfos[i].theLabel->setText(QString::number(*floatWatchInfos[i].theFloat));
	}
	for(int i = 0; i < floatWatchSliderInfos.size(); i++)	// float-slider
	{
		float prevVal = *floatWatchSliderInfos[i].theFloat;
		*floatWatchSliderInfos[i].theFloat = floatWatchSliderInfos[i].theSlider->value();
	}
	for(int i = 0; i < vec2WatchInfos.size(); i++)	// vec2
	{
		vec2WatchInfos[i].theLabel->setText("< " + QString::number(*vec2WatchInfos[i].x) + ", " + *vec2WatchInfos[i].y + " >");
	}
	for(int i = 0; i < vec3WatchInfos.size(); i++)	// vec3
	{
		vec3WatchInfos[i].theLabel->setText("< " +
			QString::number(*vec3WatchInfos[i].x) + ", " +
			QString::number(*vec3WatchInfos[i].y) + ", " +
			QString::number(*vec3WatchInfos[i].z) + " >");
	}
	for(int i = 0; i < boolWatchInfos.size(); i++)
	{
		*boolWatchInfos[i].theBool = boolWatchInfos[i].theCheckbox->checkState() == 2;
	}
}
void DebugTools::DebugWidget::ToggleLayoutOrientation(int orientation)
{
	mainLayout->setDirection(orientation == 1 ? QBoxLayout::LeftToRight : QBoxLayout::TopToBottom);
}

DebugTools::FloatWatchInfo* DebugTools::DebugWidget::WatchFloat(const char* text, const float* value)
{
	QHBoxLayout* thisLayout;
	mainLayout->addLayout(thisLayout = new QHBoxLayout);
	thisLayout->addStretch();

	thisLayout->addWidget(new QLabel(text));
	FloatWatchInfo info;
	thisLayout->addWidget(info.theLabel = new QLabel(text));
	info.theLabel->setMaximumHeight(60);
	info.theFloat = value;
	thisLayout->addStretch();

	floatWatchInfos.push_back(info);
	return &floatWatchInfos[floatWatchInfos.size() - 1];
}
DebugTools::FloatWatchSliderInfo* DebugTools::DebugWidget::WatchFloatSlider(const char* text, float* value, float min, float max)
{
	QHBoxLayout* thisLayout;
	mainLayout->addLayout(thisLayout = new QHBoxLayout);
	thisLayout->addStretch();

	thisLayout->addWidget(new QLabel(text));
	FloatWatchSliderInfo info;
	thisLayout->addWidget(info.theSlider = new Neumont::Slider(min, max));
	info.theFloat = value;
	info.theSlider->setValue(*value);
	info.theSlider->setMaximumHeight(60);
	info.theSlider->setMaximumWidth(200);
	thisLayout->addStretch();
	
	if(ActiveRenderer != 0)
	{
		connect(info.theSlider, SIGNAL(valueChanged(float)), ActiveRenderer, SLOT(RefreshRenderer()));
	}

	floatWatchSliderInfos.push_back(info);
	return &floatWatchSliderInfos[floatWatchSliderInfos.size() - 1];
}
DebugTools::BoolWatcherInfo* DebugTools::DebugWidget::WatchBool(const char* text, bool* value)
{
	QHBoxLayout* thisLayout;
	mainLayout->addLayout(thisLayout = new QHBoxLayout);
	thisLayout->addStretch();

	thisLayout->addWidget(new QLabel(text));
	BoolWatcherInfo info;
	thisLayout->addWidget(info.theCheckbox = new QCheckBox());
	info.theCheckbox->setChecked(*value);
	info.theBool = value;
	thisLayout->addStretch();

	if(ActiveRenderer != 0)
	{
		connect(info.theCheckbox, SIGNAL(stateChanged(int)), ActiveRenderer, SLOT(RefreshRenderer()));
	}

	boolWatchInfos.push_back(info);
	return &boolWatchInfos[boolWatchInfos.size() - 1];
}
DebugTools::Vec2WatchInfo* DebugTools::DebugWidget::WatchVec2(const char* text, float* x, float* y)
{
	QHBoxLayout* thisLayout;
	mainLayout->addLayout(thisLayout = new QHBoxLayout);
	thisLayout->addStretch();

	thisLayout->addWidget(new QLabel(text));
	Vec2WatchInfo info;
	thisLayout->addWidget(info.theLabel = new QLabel());
	info.theLabel->setMaximumHeight(60);
	info.x = x;
	info.y = y;
	thisLayout->addStretch();

	vec2WatchInfos.push_back(info);
	return &vec2WatchInfos[vec2WatchInfos.size() - 1];
}
DebugTools::Vec3WatchInfo* DebugTools::DebugWidget::WatchVec3(const char* text, float* x, float* y, float* z)
{
	QHBoxLayout* thisLayout;
	mainLayout->addLayout(thisLayout = new QHBoxLayout);
	thisLayout->addStretch();

	thisLayout->addWidget(new QLabel(text));
	Vec3WatchInfo info;
	thisLayout->addWidget(info.theLabel = new QLabel());
	info.theLabel->setMaximumHeight(60);
	info.x = x;
	info.y = y;
	info.z = z;
	thisLayout->addStretch();

	vec3WatchInfos.push_back(info);
	return &vec3WatchInfos[vec3WatchInfos.size() - 1];
}
DebugTools::ButtonWatchInfo** DebugTools::DebugWidget::WatchButton(const char* text, Delegate function)
{
	QHBoxLayout* thisLayout;
	mainLayout->addLayout(thisLayout = new QHBoxLayout);
	thisLayout->addStretch();

	ButtonWatchInfo info;
	thisLayout->addWidget(info.theButton = new QPushButton(text));
	info.ConnectButton();
	info.buttonFunction = function;
	thisLayout->addStretch();
	
	buttonWatchInfos.push_back(&info);
	return &buttonWatchInfos[buttonWatchInfos.size() - 1];
}

void DebugTools::ButtonWatchInfo::ConnectButton()
{
	qDebug() << "connecting button!";
	connect(theButton, SIGNAL(released()), this, SLOT(functioncall()));
}
void DebugTools::ButtonWatchInfo::functioncall()
{
	qDebug() << "button pressed!";
	buttonFunction();
}

#include "DebugApplication_moc.cpp"