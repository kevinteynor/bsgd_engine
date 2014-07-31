#include "NormalDisplay.h"

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <ShapeData.h>
#include <ShapeGenerator.h>
#include <ShaderManager.h>
#include <Qt\qdebug.h>

#include <Qt\qfile.h>
#include <Qt\qtextstream.h>

void NormalDisplay::Initialize()
{
	DebugApplication::Initialize();

	diffuseAmount = 0.5f;
	specAmount = 0.25f;
}
void NormalDisplay::RendererInitialized()
{
	DebugApplication::RendererInitialized();

	renderer->LoadTexture("Flat.png", flatNormalID);
	renderer->LoadTexture("Shapes.png", shapesNormalID);

	//CreatePlane();
	CreateCube();

	DebugWidget::WatchFloatSlider("Diffuse %", &diffuseAmount, 0, 1);
	DebugWidget::WatchFloatSlider("Specular %", &specAmount, 0, 1);	

	DebugWidget::WatchFloatSlider("Light X: ", &lightPos.x, -10, 10);
	DebugWidget::WatchFloatSlider("Light Y: ", &lightPos.y, -10, 10);
	DebugWidget::WatchFloatSlider("Light Z: ", &lightPos.z, -10, 10);

	

	DebugWidget::WatchBool("Use Shapes Normal Map", &shapesNormals);
}

void NormalDisplay::Update()
{
	DebugApplication::Update();

	renderer->lightPosition = lightPos;

	if(!(renderer->RendererInitialized && shapesCreated)) return;
	
	if(shapesNormals)
	{
		renderer->normalMapTexture = shapesNormalID;
	} 
	else 
	{
		renderer->normalMapTexture = flatNormalID;
	}

	DebugShapes::SetShapeTransform(lightID, glm::translate(lightPos) * glm::scale(0.25f, 0.25f, 0.25f));
	shapeR->UpdateUniform(diffValID, &diffuseAmount, sizeof(float));
	shapeR->UpdateUniform(specValID, &specAmount, sizeof(float));
	shapeR->UpdateUniform(lightPosID, &lightPos, sizeof(glm::vec3));
	shapeR->UpdateUniform(camPosID, &renderer->camera.position, sizeof(glm::vec3));
}

void NormalDisplay::CreatePlane()
{
	shaderIndex = renderer->AddShader("VertexShader.sh", "FragShader.sh");

	shapePos = glm::vec3(0, 0, 0);
	lightPos = glm::vec3(-1, -2, 5);

	renderer->camera.SetPosition(lightPos);
	renderer->camera.SetTarget(glm::vec3(0,0,0) - lightPos);

	lightID = DebugShapes::MakeCube(glm::translate(lightPos), glm::vec4(1.0f,1.0f,1.0f,0.5f), rl_TRANSPARENT);

	Neumont::ShapeData data = Neumont::ShapeGenerator::makePlane(10, 10);
	uint planeGeo = renderer->AddGeometry(data.verts, data.numVerts, data.indices, data.numIndices, rm_TRIANGLES);
	shapeR = renderer->AddRenderableInstance_p(planeGeo, glm::translate(shapePos) * glm::rotate(-90.0f, 1.0f,0.0f,0.0f), shaderIndex);
	shapeR->AddUniform("mvp", ut_MAT4_MVP, 0, 0);
	shapeR->AddUniform("modelToWorld", ut_MAT4, &(glm::translate(shapePos) * glm::rotate(-90.0f, 1.0f,0.0f,0.0f)), sizeof(glm::mat4));
	shapeR->AddUniform("modelToWorld_Normal", ut_MAT3, &glm::mat3(glm::rotate(-90.0f, 1.0f,0.0f,0.0f)), sizeof(glm::mat3));

	lightPosID = shapeR->AddUniform("light_position", ut_VEC3, &lightPos, sizeof(glm::vec3));
	camPosID = shapeR->AddUniform("eye_pos", ut_VEC3, &renderer->camera.position, sizeof(glm::vec3));
	diffValID = shapeR->AddUniform("diffVal", ut_FLOAT, &diffuseAmount, sizeof(float));
	specValID = shapeR->AddUniform("specVal", ut_FLOAT, &specAmount, sizeof(float));

	shapesCreated = true;
}

void NormalDisplay::CreateCube()
{
	shaderIndex = renderer->AddShader("cubeVS.sh", "cubeFS.sh");

	shapePos = glm::vec3(0,0,0);
	lightPos = glm::vec3(0,0,-5);

	renderer->camera.SetPosition(glm::vec3(0,0,-10));
	renderer->camera.SetTarget(glm::vec3(0,0,1));

	lightID = DebugShapes::MakeCube(glm::translate(lightPos), glm::vec4(1.0f, 1.0f, 1.0f, 0.5f), rl_TRANSPARENT);

	Neumont::ShapeData data = Neumont::ShapeGenerator::makeCube();
	uint cubeGeo = renderer->AddGeometry(data.verts, data.numVerts, data.indices, data.numIndices, rm_TRIANGLES);
	shapeR = renderer->AddRenderableInstance_p(cubeGeo, glm::translate(shapePos) * glm::scale(3.0f, 3.0f, 3.0f), shaderIndex);

	shapeR->AddUniform("m", ut_MAT4_Model, 0, 0);
	shapeR->AddUniform("v", ut_MAT4_View, 0, 0);
	shapeR->AddUniform("p", ut_MAT4_Projection, 0, 0);

	lightPosID = shapeR->AddUniform("light_position", ut_VEC3, &lightPos, sizeof(glm::vec3));
	camPosID = shapeR->AddUniform("eye_pos", ut_VEC3, &renderer->camera.position, sizeof(glm::vec3));
	diffValID = shapeR->AddUniform("diffVal", ut_FLOAT, &diffuseAmount, sizeof(float));
	specValID = shapeR->AddUniform("specVal", ut_FLOAT, &specAmount, sizeof(float));

	shapesCreated = true;
}

#include "NormalDisplay_moc.cpp"