#include "ParentWindow.h"
#include <glm\gtx\transform.hpp>
#include "ShapeGenerator.h"
#include "Qt\qdebug.h"
#include "DebugShapes.h"
#include "InputManager.h"

#define DRAW_NON_DEBUG_SHAPES true

ParentWindow::ParentWindow(void)
{
	renderer = new Renderer();
	connect(renderer, SIGNAL(rendererInitialized()), this, SLOT(InitializeComponents()));
	connect(renderer, SIGNAL(preDraw()), this, SLOT(UpdateUniforms()));

	UpdateClock::Initialize();

	updateTimer = new QTimer(this);
	connect(updateTimer, SIGNAL(timeout()), this, SLOT(Update()));
	updateTimer->start();

	DebugShapes::SetRenderer(renderer);
}
ParentWindow::~ParentWindow(void)
{

}

void ParentWindow::InitializeRenderer()
{
	renderer->show();
	renderer->resize(600,400);
	renderer->update();

	renderer->VERTEX_SIZE = sizeof(Neumont::Vertex);
	renderer->INDEX_SIZE = sizeof(ushort);
	renderer->POSITION_OFFSET = Neumont::Vertex::POSITION_OFFSET;
	renderer->COLOR_OFFSET = Neumont::Vertex::COLOR_OFFSET;
	renderer->NORMAL_OFFSET = Neumont::Vertex::NORMAL_OFFSET;
	renderer->UV_OFFSET = Neumont::Vertex::UV_OFFSET;
}
void ParentWindow::InitializeComponents()
{
	CreateShapes();
}

void ParentWindow::CreateShapes()
{
	if(DRAW_NON_DEBUG_SHAPES)
	{
#pragma region GEOMETRIES

	Neumont::ShapeData shapeData = Neumont::ShapeGenerator::makeCube();
	cubeGeometryID = renderer->AddGeometry(shapeData.verts, shapeData.numVerts, shapeData.indices, shapeData.numIndices, rm_TRIANGLES);
	
	shapeData = Neumont::ShapeGenerator::makeSphere(35);
	sphereGeometryID = renderer->AddGeometry(shapeData.verts, shapeData.numVerts, shapeData.indices, shapeData.numIndices, rm_TRIANGLES);

	shapeData = Neumont::ShapeGenerator::makeArrow();
	arrowGeometryID = renderer->AddGeometry(shapeData.verts, shapeData.numVerts, shapeData.indices, shapeData.numIndices, rm_TRIANGLES);

	shapeData = Neumont::ShapeGenerator::makeTorus(35);
	torusGeometryID = renderer->AddGeometry(shapeData.verts, shapeData.numVerts, shapeData.indices, shapeData.numIndices, rm_TRIANGLES);

#pragma endregion GEOMETRIES
#pragma region SHADERS
	
	shader1ID = renderer->AddShader("shader_vertex1.sh", "shader_frag1.sh");
	shader2ID = renderer->AddShader("shader_vertex2.sh", "shader_frag2.sh");

#pragma endregion SHADERS
#pragma region RENDERABLE INSTANCES

	Renderer::RenderableInstanceInfo* newRenderable;
	
	// material colors
	newRenderable = renderer->AddRenderableInstance_p(cubeGeometryID, glm::translate(1.5f, 1.5f, 0.0f), shader1ID);
	newRenderable->AddUniform("mvp", ut_MAT4_MVP, 0, 0);
	newRenderable = renderer->AddRenderableInstance_p(sphereGeometryID, glm::translate(-1.5f, 1.5f, 0.0f), shader1ID);
	newRenderable->AddUniform("mvp", ut_MAT4_MVP, 0, 0);
	newRenderable = renderer->AddRenderableInstance_p(arrowGeometryID, glm::translate(4.5f, 1.5f, 0.0f), shader1ID);
	newRenderable->AddUniform("mvp", ut_MAT4_MVP, 0, 0);
	newRenderable = renderer->AddRenderableInstance_p(torusGeometryID, glm::translate(-4.5f, 1.5f, 0.0f), shader1ID);
	newRenderable->AddUniform("mvp", ut_MAT4_MVP, 0, 0);

	// blue
	newRenderable = renderer->AddRenderableInstance_p(cubeGeometryID, glm::translate(1.5f, 4.5f, 0.0f), shader2ID);
	newRenderable->AddUniform("mvp", ut_MAT4_MVP, 0, 0);
	newRenderable = renderer->AddRenderableInstance_p(sphereGeometryID, glm::translate(-1.5f, 4.5f, 0.0f), shader2ID);
	newRenderable->AddUniform("mvp", ut_MAT4_MVP, 0, 0);
	newRenderable = renderer->AddRenderableInstance_p(arrowGeometryID, glm::translate(4.5f, 4.5f, 0.0f), shader2ID);
	newRenderable->AddUniform("mvp", ut_MAT4_MVP, 0, 0);
	newRenderable = renderer->AddRenderableInstance_p(torusGeometryID, glm::translate(-4.5f, 4.5f, 0.0f), shader2ID);
	newRenderable->AddUniform("mvp", ut_MAT4_MVP, 0, 0);

#pragma endregion RENDERABLE INSTANCES
	}

#pragma region DEBUG SHAPES
	DebugShapes::MakeArrow(glm::translate(0.0f,0.0f,0.0f), 5, true, glm::vec3(1.0f, 0.0f, 0.0f));
	DebugShapes::MakeCube(glm::translate(10.0f, 2.0f, -3.0f), 7, true, glm::vec3(0.0f, 1.0f, 0.0f));
	DebugShapes::MakePlane(glm::translate(0.0f, 0.0f, -4.0f), 9, true, glm::vec3(0.0f, 0.0f, 1.0f), 2);
	DebugShapes::MakeLine(glm::translate(3.0f, 1.0f, -1.0f), INDEFINITE_DURATION, true, glm::vec3(1.0f, 1.0f, 1.0f));
	
	DebugShapes::MakeCrosshair(glm::translate(5.0f, 0.0f, 0.0f), INDEFINITE_DURATION, false);
#pragma endregion
}

void ParentWindow::UpdateUniforms()
{	
}

void ParentWindow::Update()
{
	UpdateClock::Update();
	float deltaTime = UpdateClock::DeltaTime();

	//if(InputManager::IsKeyDown(VK_SHIFT))
	//{
	//
	//}

	DebugShapes::Update(deltaTime);

	renderer->repaint();
}