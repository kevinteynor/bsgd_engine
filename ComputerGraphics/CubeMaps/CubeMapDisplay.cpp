#include "CubeMapDisplay.h"
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <ShapeGenerator.h>
#include <ShapeData.h>


void CubeMapDisplay::Initialize()
{
	DebugApplication::Initialize();
}
void CubeMapDisplay::RendererInitialized()
{
	DebugApplication::RendererInitialized();

	CreateSphere();
	CreateCube();

	DebugWidget::WatchFloatSlider("Reflectivity", &reflectivity, 0, 1);
}

void CubeMapDisplay::CreateSphere()
{
	// create the shader
	sphereShaderIndex = renderer->AddShader("sphereVert.sh", "sphereFrag.sh");

	spherePos = glm::vec3(0,0,0);

	drawCubeMap = true;
	reflectivity = 0.75f;

	renderer->camera.SetPosition(0,0,3);

	//renderer->LoadCubemap(cubeMapTextureID, "x_pos.png", "x_neg.png", "y_pos.png", "y_neg.png", "z_pos.png", "z_neg.png");
	renderer->LoadCubemap(cubeMapTextureID, "posx.png", "negx.png", "posy.png", "negy.png", "posz.png", "negz.png");

	Neumont::ShapeData data = Neumont::ShapeGenerator::makeSphere(15);
	uint sphereGeo = renderer->AddGeometry(data.verts, data.numVerts, data.indices, data.numIndices, rm_TRIANGLES);
	sphereInfo = renderer->AddRenderableInstance_p(sphereGeo, glm::translate(spherePos), sphereShaderIndex);
	sphereInfo->AddUniform("MVP", ut_MAT4_MVP, 0, 0);
	sphereInfo->AddUniform("M", ut_MAT4_Model, 0, 0);
	sphereInfo->AddUniform("V", ut_MAT4_View, 0, 0);
	sphereInfo->AddUniform("P", ut_MAT4_Projection, 0, 0);
	eyePosID = sphereInfo->AddUniform("EyePosition", ut_VEC3, &renderer->camera.position, sizeof(glm::vec3));
	reflectivityID = sphereInfo->AddUniform("Reflectivity", ut_FLOAT, &reflectivity, sizeof(float));
	cubeMapUniformID = sphereInfo->AddUniform("CubeMapTex", ut_CUBE_MAP, &cubeMapTextureID, sizeof(uint));
}

void CubeMapDisplay::CreateCube()
{
	cubeShaderIndex = renderer->AddShader("cubeVert.sh", "cubeFrag.sh");

	spherePos = glm::vec3(0,0,0);

	Neumont::ShapeData data = Neumont::ShapeGenerator::makeCube();
	uint cubeGeo = renderer->AddGeometry(data.verts, data.numVerts, data.indices, data.numIndices, rm_TRIANGLES);
	cubeInfo = renderer->AddRenderableInstance_p(cubeGeo, glm::translate(spherePos) * glm::scale(15.0f,15.0f,15.0f), cubeShaderIndex);
	cubeInfo->AddUniform("MVP", ut_MAT4_MVP, 0, 0);
	cubeInfo->AddUniform("CubeMapTex", ut_CUBE_MAP, &cubeMapTextureID, sizeof(uint));

	shapeCreated = true;
}

void CubeMapDisplay::Update()
{
	DebugApplication::Update();

	if(!(renderer->RendererInitialized && shapeCreated)) return;

	sphereInfo->UpdateUniform(reflectivityID, &reflectivity, sizeof(float));
	sphereInfo->UpdateUniform(eyePosID, &renderer->camera.position, sizeof(glm::vec3));
}
#include "CubeMapDisplay_moc.cpp"