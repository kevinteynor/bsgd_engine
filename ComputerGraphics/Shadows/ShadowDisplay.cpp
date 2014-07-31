#include "ShadowDisplay.h"

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <ShapeData.h>
#include <ShapeGenerator.h>
#include <ShaderManager.h>
#include <Qt\qdebug.h>

void ShadowDisplay::Initialize()
{
	DebugApplication::Initialize();

	cubePos = glm::vec3(0,0,-1);
	spherePos = glm::vec3(3,0,5);
	arrowPos = glm::vec3(-1,0,1);
	planePos = glm::vec3(0,-1,-2);
	lightPos = glm::vec3(0,2,4);

	renderer->CreateShadowMap = true;
}

void ShadowDisplay::RendererInitialized()
{
	DebugApplication::RendererInitialized();
	lightID = DebugShapes::MakeSphere(glm::translate(lightPos) * glm::scale(0.25f,0.25f,0.25f), glm::vec4(1,1,0,0.75f), 3, rl_TRANSPARENT);

	CreateShapes();

	DebugWidget::WatchFloatSlider("Light X", &lightPos.x);
	DebugWidget::WatchFloatSlider("Light Y", &lightPos.y);
	DebugWidget::WatchFloatSlider("Light Z", &lightPos.z);
}

void ShadowDisplay::Update()
{
	DebugApplication::Update();

	if(lightID > 1000) return;
	DebugShapes::SetShapeTransform(lightID, glm::translate(lightPos));
	renderer->lightPosition = lightPos;
	renderer->lightDirection = glm::vec3(0,0,0) - lightPos;  // glm::vec3(0,-0.2f,-0.8f);
}

void ShadowDisplay::CreateShapes()
{
	Neumont::ShapeData data;

	// add the shader
	shaderID = renderer->AddShader("VertexShader.sh", "FragShader.sh");
	qDebug() << "SHADER CREATED: " << shaderID;

	// sphere
	data = Neumont::ShapeGenerator::makeCube();
	uint sphereGEO = renderer->AddGeometry(data.verts, data.numVerts, data.indices, data.numIndices, rm_TRIANGLES);
	sphereR = renderer->AddRenderableInstance_p(sphereGEO, glm::translate(spherePos), shaderID);
	sphereR->AddUniform("mvp", ut_MAT4_MVP, 0, 0);
	sphereR->AddUniform("modelToWorld", ut_MAT4, &glm::translate(spherePos), sizeof(glm::mat4));
	sphereR->AddUniform("modelToWorld_Normal", ut_MAT3, &glm::mat3(glm::translate(spherePos)), sizeof(glm::mat3));
	sphereR->AddUniform("light_position", ut_VEC3, &lightPos, sizeof(glm::vec3));
	sphereR->AddUniform("eye_pos", ut_VEC3, &renderer->camera.position, sizeof(glm::vec3));

	// cube
	data = Neumont::ShapeGenerator::makeCube();
	uint cubeGEO = renderer->AddGeometry(data.verts, data.numVerts, data.indices, data.numIndices, rm_TRIANGLES);
	cubeR = renderer->AddRenderableInstance_p(cubeGEO, glm::translate(cubePos), shaderID);
	cubeR->AddUniform("mvp", ut_MAT4_MVP, 0, 0);
	cubeR->AddUniform("modelToWorld", ut_MAT4, &glm::translate(cubePos), sizeof(glm::mat4));
	cubeR->AddUniform("modelToWorld_Normal", ut_MAT3, &glm::mat3(glm::translate(cubePos)), sizeof(glm::mat3));
	cubeR->AddUniform("light_position", ut_VEC3, &lightPos, sizeof(glm::vec3));
	cubeR->AddUniform("eye_pos", ut_VEC3, &renderer->camera.position, sizeof(glm::vec3));

	// arrow
	data = Neumont::ShapeGenerator::makeArrow();
	uint arrowGEO = renderer->AddGeometry(data.verts, data.numVerts, data.indices, data.numIndices, rm_TRIANGLES);
	arrowR = renderer->AddRenderableInstance_p(arrowGEO, glm::translate(arrowPos), shaderID);
	arrowR->AddUniform("mvp", ut_MAT4_MVP, 0, 0);
	arrowR->AddUniform("modelToWorld", ut_MAT4, &glm::translate(arrowPos), sizeof(glm::mat4));
	arrowR->AddUniform("modelToWorld_Normal", ut_MAT3, &glm::mat3(glm::translate(arrowPos)), sizeof(glm::mat3));
	arrowR->AddUniform("light_position", ut_VEC3, &lightPos, sizeof(glm::vec3));
	arrowR->AddUniform("eye_pos", ut_VEC3, &renderer->camera.position, sizeof(glm::vec3));

	// plane
	data = Neumont::ShapeGenerator::makePlane(5,5);
	uint planeGEO = renderer->AddGeometry(data.verts, data.numVerts, data.indices, data.numIndices, rm_TRIANGLES);
	planeR = renderer->AddRenderableInstance_p(planeGEO, glm::translate(planePos), shaderID);
	planeR->AddUniform("mvp", ut_MAT4_MVP, 0, 0);
	planeR->AddUniform("modelToWorld", ut_MAT4, &glm::translate(planePos), sizeof(glm::mat4));
	planeR->AddUniform("modelToWorld_Normal", ut_MAT3, &glm::mat3(glm::translate(planePos)), sizeof(glm::mat3));
	planeR->AddUniform("light_position", ut_VEC3, &lightPos, sizeof(glm::vec3));
	planeR->AddUniform("eye_pos", ut_VEC3, &renderer->camera.position, sizeof(glm::vec3));
}

#include "ShadowDisplay_moc.cpp"