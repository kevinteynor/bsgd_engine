#define USE_DEBUG_MENUS

#include <DebugCamera.h>
#include <DebugMenu.h>
#include <DebugShapes.h>
#include <fstream>
#include <glm\gtx\transform.hpp>
#include <Qt\qtimer.h>
#include <QtGui\qvboxlayout>
#include <Renderer.h>
#include <ShapeData.h>
#include <ShapeGenerator.h>
#include "ParentWindow.h"

ParentWindow::ParentWindow(){}
ParentWindow::~ParentWindow()
{

	//Shutdown();
	//delete renderer;
	//delete updateTimer;
	//delete playerCamera;
	//delete debugCamera;
	//
	//renderer = 0;
	//updateTimer = 0;
	//playerCamera = 0;
	//debugCamera = 0;
}

void ParentWindow::Initialize()
{
	QVBoxLayout* mainLayout;
	QHBoxLayout* debugLayout;
	mainWindow = new QWidget;
	mainWindow->setLayout(mainLayout = new QVBoxLayout);
	mainLayout->addLayout(debugLayout = new QHBoxLayout);
	mainLayout->addWidget(renderer = new Renderer);

	DebugMenu::Initialize(debugLayout);

	InitializeRenderer();
	connect(renderer, SIGNAL(rendererInitialized()), this, SLOT(InitializeComponents()));

	updateTimer = new QTimer(this);
	connect(updateTimer, SIGNAL(timeout()), this, SLOT(Update()));
	updateTimer->start();

	//mainWindow->setMaximumHeight(500);
	renderer->setMinimumSize(800, 800);
	mainWindow->show();
	renderer->show();

	DebugShapes::SetRenderer(renderer);
	
	UpdateClock::Initialize();
}
void ParentWindow::InitializeRenderer()
{
	renderer->VERTEX_SIZE = sizeof(Neumont::Vertex);
	renderer->INDEX_SIZE = sizeof(ushort);
	renderer->POSITION_OFFSET = Neumont::Vertex::POSITION_OFFSET;
	renderer->COLOR_OFFSET = Neumont::Vertex::COLOR_OFFSET;
	renderer->NORMAL_OFFSET = Neumont::Vertex::NORMAL_OFFSET;
	renderer->UV_OFFSET = Neumont::Vertex::UV_OFFSET;
}
void ParentWindow::InitializeComponents()
{
	uint shaderID = renderer->AddShader("shader_vertex1.sh", "shader_frag1.sh");
	Neumont::ShapeData sData = Neumont::ShapeGenerator::makeSphere(15);
	uint sphereGeoID = renderer->AddGeometry(sData.verts, sData.numVerts, sData.indices, sData.numIndices, rm_LINES);

	Renderer::RenderableInstanceInfo* instance = renderer->AddRenderableInstance_p(sphereGeoID, glm::translate(0.0f, 0.0f, 0.0f), shaderID);
	instance->AddUniform("mvp", ut_MAT4_MVP, 0, 0);

	DebugMenu::WatchFloatSlider("shpere position-X:", &(instance->position[3][0]));
	DebugMenu::WatchFloatSlider("shpere position-Y:", &(instance->position[3][1]));
	DebugMenu::WatchFloatSlider("shpere position-Z:", &(instance->position[3][2]));

	DebugShapes::MakeArrow(glm::translate(0.0f,0.0f,0.0f), INDEFINITE_DURATION, true, glm::vec3(1.0f, 0.0f, 0.0f));
	DebugShapes::MakeCube(glm::translate(3.0f, 0.0f, 0.0f), INDEFINITE_DURATION, true, glm::vec3(0.0f, 1.0f, 0.0f));

#pragma region MODEL FILE LOADING
	std::ifstream in("Annie.bin", std::ios::in | std::ios::binary);
	in.seekg(0, std::ios::end);

	const unsigned int FILE_SIZE = in.tellg();
	char* buf = new char[FILE_SIZE];
	in.seekg(0, std::ios::beg);
	in.read(buf, FILE_SIZE);
	in.close();
	
	unsigned int* shapeCount = reinterpret_cast<unsigned int*>(buf);

	Neumont::ShapeData* shapeDatas = reinterpret_cast<Neumont::ShapeData*>(buf + sizeof(unsigned int));

	for(int i = 0; i < 2; i++)
	{
		Neumont::ShapeData& d = shapeDatas[i];
		d.verts = reinterpret_cast<Neumont::Vertex*>(buf + (unsigned int)(d.verts));
		d.indices = reinterpret_cast<unsigned short*>(buf + (unsigned int)(d.indices));

		uint geoID = renderer->AddGeometry(d.verts, d.numVerts, d.indices, d.numIndices, rm_TRIANGLES);
		instance = renderer->AddRenderableInstance_p(geoID, glm::translate(0.0f, 0.0f, 0.0f), shaderID);
		instance->AddUniform("mvp", ut_MAT4_MVP, 0, 0);
	}

	delete [] buf;

#pragma endregion
}

void ParentWindow::Shutdown()
{
	delete mainWindow;
	mainWindow = 0;

	delete renderer;
	renderer = 0;
}

void ParentWindow::Update()
{
	UpdateClock::Update();

	renderer->update();
	DebugShapes::Update(UpdateClock::DeltaTime());
	DebugMenu::Update();
}