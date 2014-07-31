#include "ParentWindow.h"
#include <glm\gtx\transform.hpp>
#include "ShapeGenerator.h"
#include "Qt\qdebug.h"

ParentWindow::ParentWindow(void)
{
	renderer = new Renderer();
	connect(renderer, SIGNAL(rendererInitialized()), this, SLOT(InitializeComponents()));
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
#pragma region GEOMETRIES

	Neumont::ShapeData shapeData = Neumont::ShapeGenerator::makeCube();
	cubeGeometryID = renderer->AddGeometry(shapeData.verts, shapeData.numVerts, shapeData.indices, shapeData.numIndices);
	
	shapeData = Neumont::ShapeGenerator::makeSphere(35);
	sphereGeometryID = renderer->AddGeometry(shapeData.verts, shapeData.numVerts, shapeData.indices, shapeData.numIndices);

	shapeData = Neumont::ShapeGenerator::makeArrow();
	arrowGeometryID = renderer->AddGeometry(shapeData.verts, shapeData.numVerts, shapeData.indices, shapeData.numIndices);

	shapeData = Neumont::ShapeGenerator::makeTorus(35);
	torusGeometryID = renderer->AddGeometry(shapeData.verts, shapeData.numVerts, shapeData.indices, shapeData.numIndices);

#pragma endregion GEOMETRIES
#pragma region SHADERS

	Renderer::ShaderUniformNames uniforms;
	uniforms.mat4_mvp = "mvp";

	shader1ID = renderer->AddShader("shader_vertex1.sh", "shader_frag1.sh", uniforms);

	uniforms.mat4_mvp = "mvp";

	shader2ID = renderer->AddShader("shader_vertex2.sh", "shader_frag2.sh", uniforms);

	uniforms.mat4_mvp = "mvp";
	uniforms.vec3_lightColor = "light_color";
	uniforms.vec3_lightPos = "light_position";
	uniforms.vec3_eyePos = "eye_pos";
	uniforms.mat4_modelToWorld = "modelToWorld";
	uniforms.mat4_modelToWorld_Normal = "modelToWorld_norm";

	shader3ID = renderer->AddShader("shader_vertex3.sh", "shader_frag3.sh", uniforms);
	shader4ID = renderer->AddShader("shader_vertex4.sh", "shader_frag4.sh", uniforms);

#pragma endregion SHADERS
#pragma region RENDERABLE INSTANCES

	// vertex shader lighting
	renderer->AddRenderableInstance(cubeGeometryID, glm::translate(1.5f, -4.5f, 0.0f), shader3ID);
	renderer->AddRenderableInstance(sphereGeometryID, glm::translate(-1.5f, -4.5f, 0.0f), shader3ID);
	renderer->AddRenderableInstance(arrowGeometryID, glm::translate(4.5f, -4.5f, 0.0f), shader3ID);
	renderer->AddRenderableInstance(torusGeometryID, glm::translate(-4.5f, -4.5f, 0.0f), shader3ID);

	// fragment shader lighting
	renderer->AddRenderableInstance(cubeGeometryID, glm::translate(1.5f, -1.5f, 0.0f), shader4ID);
	renderer->AddRenderableInstance(sphereGeometryID, glm::translate(-1.5f, -1.5f, 0.0f), shader4ID);
	renderer->AddRenderableInstance(arrowGeometryID, glm::translate(4.5f, -1.5f, 0.0f), shader4ID);
	renderer->AddRenderableInstance(torusGeometryID, glm::translate(-4.5f, -1.5f, 0.0f), shader4ID);
	
	// material colors
	renderer->AddRenderableInstance(cubeGeometryID, glm::translate(1.5f, 1.5f, 0.0f), shader1ID);
	renderer->AddRenderableInstance(sphereGeometryID, glm::translate(-1.5f, 1.5f, 0.0f), shader1ID);
	renderer->AddRenderableInstance(arrowGeometryID, glm::translate(4.5f, 1.5f, 0.0f), shader1ID);
	renderer->AddRenderableInstance(torusGeometryID, glm::translate(-4.5f, 1.5f, 0.0f), shader1ID);

	// blue
	renderer->AddRenderableInstance(cubeGeometryID, glm::translate(1.5f, 4.5f, 0.0f), shader2ID);
	renderer->AddRenderableInstance(sphereGeometryID, glm::translate(-1.5f, 4.5f, 0.0f), shader2ID);
	renderer->AddRenderableInstance(arrowGeometryID, glm::translate(4.5f, 4.5f, 0.0f), shader2ID);
	renderer->AddRenderableInstance(torusGeometryID, glm::translate(-4.5f, 4.5f, 0.0f), shader2ID);

#pragma endregion RENDERABLE INSTANCES
}