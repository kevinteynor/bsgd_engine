#include <GL\glew.h>

#include "SceneWindow.h"

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <ShapeGenerator.h>
#include <NativeModel.h>
#include <noise\noise.h>
#include <time.h>
#include <FbxLoader.h>

#define MAX_SNOWMEN_RENDERABLES 6

SceneWindow::SceneWindow() : initialized(false)
{
	renderables = new Renderer::RenderableInstanceInfo*[MAX_SNOWMEN_RENDERABLES];
	renderablesCount = 0;
}
SceneWindow::~SceneWindow()
{
	delete [] renderables;
}

void SceneWindow::Initialize()
{
	DebugApplication::Initialize();

	renderer->CreateShadowMap = true;
}
void SceneWindow::RendererInitialized()
{
	DebugApplication::RendererInitialized();

	renderer->lightPosition = glm::vec3(-10.0f,10.0f,-2.5f);
	renderer->lightDirection = glm::vec3(-0.25f, -0.75f, 0.0f);

	cubemapShaderID = renderer->AddShader("cmvertex.sh", "cmfragment.sh");

	useLighting = 1;	// true
	useNormalMap = 1;	// true
	useShadows = 1;
	uselighting_b = true;
	usenormal_b = true;
	useshadows_b = true;

	meltedLevel = 0.0f;
	color = glm::vec4(1,1,1,1);

	DebugWidget::WatchBool("Normal Map/Texture", &usenormal_b);
	DebugWidget::WatchBool("Phong Lighting    ", &uselighting_b);
	DebugWidget::WatchBool("Shadows           ", &useshadows_b);

	DebugWidget::WatchFloatSlider("light X", &renderer->lightPosition.x, -15, 15);
	DebugWidget::WatchFloatSlider("light Y", &renderer->lightPosition.y, -5, 15);
	DebugWidget::WatchFloatSlider("light Z", &renderer->lightPosition.z, -15, 15);
	DebugWidget::WatchFloatSlider("alpha", &color.a, 0.0f, 1.0f);
	DebugWidget::WatchFloatSlider("melted %", &meltedLevel, 0.0f, 1.0f);

	CreateShapes();
	CreateNoise();

	initialized = true;
}

void SceneWindow::CreateShapes()
{
	shaderID = renderer->AddShader("vertex.sh", "fragment.sh");	

	renderer->LoadTexture("snow.png", snowTextureID);
	renderer->LoadTexture("snownorm.png", snowNormalID);
	renderer->normalMapTexture = snowNormalID;


	// cubemap
	Neumont::ShapeData data = Neumont::ShapeGenerator::makeCube();
	uint cubeGeoID = renderer->AddGeometry(data.verts, data.numVerts, data.indices, data.numIndices, rm_TRIANGLES);
	renderer->LoadCubemap(cubemapTextureID, "posx.png", "negx.png", "posy.png", "negy.png", "posz.png", "negz.png");
	cubemapCubeRenderable = renderer->AddRenderableInstance_p(cubeGeoID, glm::scale(200.0f, 200.0f, 200.0f), cubemapShaderID);
	cubemapCubeRenderable->AddUniform("MVP", ut_MAT4_MVP, 0, 0);
	cubemapCubeRenderable->AddUniform("CubeMapTex", ut_CUBE_MAP, &cubemapTextureID, sizeof(uint));

	Neumont::ShapeData* datap;
	uint count = 0;
	Neumont::FbxLoader::loadFbxFile("sphere.fbx", datap, count);
	sphereGeoID = renderer->AddGeometry(datap->verts, datap->numVerts, datap->indices, datap->numIndices, rm_TRIANGLES);
	
	CreateSnowman(glm::vec3(2,0,-3), 1);
	CreateSnowman(glm::vec3(-2,0,2), 1.3f);
	CreateSnowman(glm::vec3(-3,0,-1), 0.8f);

	lightShapeID = DebugShapes::MakeSphere(glm::translate(renderer->lightPosition) * glm::scale(0.2f, 0.2f, 0.2f), glm::vec4(1,1,1,0.4f), 6, rl_TOPMOST);

	data = Neumont::ShapeGenerator::makePlane(12,12);
	uint planeGeoID = renderer->AddGeometry(data.verts, data.numVerts, data.indices, data.numIndices, rm_TRIANGLES);
	planeRenderable = renderer->AddRenderableInstance_p(planeGeoID, glm::translate(0.0f, -1.0f, 0.0f), shaderID);
	planeRenderable->AddUniform("MVP", ut_MAT4_MVP, 0, 0);
	planeRenderable->AddUniform("LightPos", ut_VEC3, &renderer->lightPosition, sizeof(glm::vec3));
	planeRenderable->AddUniform("EyePos", ut_VEC3, &renderer->camera.position, sizeof(glm::vec3));
	planeRenderable->AddUniform("UseLighting", ut_INT, &useLighting, sizeof(int));
	planeRenderable->AddUniform("UseNormalMap", ut_INT, &useNormalMap, sizeof(int));
	planeRenderable->AddUniform("UseShadows", ut_INT, &useShadows, sizeof(int));
	planeRenderable->AddUniform("meltedLevel", ut_FLOAT, &meltedLevel, sizeof(float));
	planeRenderable->AddUniform("color", ut_VEC4, &color, sizeof(glm::vec4));
	planeRenderable->AddUniform("M", ut_MAT4_Model, 0, 0);
	planeRenderable->AddUniform("V", ut_MAT4_View, 0, 0);
	planeRenderable->AddUniform("P", ut_MAT4_Projection, 0, 0);
	planeRenderable->AddUniform("textureMap", ut_TEXTURE, &snowTextureID, sizeof(uint));
	planeRenderable->layer = rl_TRANSPARENT;
}

void SceneWindow::CreateSnowman(glm::vec3 pos, float scale)
{
	if(renderablesCount >= MAX_SNOWMEN_RENDERABLES - 1) return;

	renderables[renderablesCount] = renderer->AddRenderableInstance_p(sphereGeoID, glm::translate(pos) * glm::scale(scale, scale, scale), shaderID);
	renderables[renderablesCount]->AddUniform("MVP", ut_MAT4_MVP, 0, 0);
	lightPosUniformID = renderables[renderablesCount]->AddUniform("LightPos", ut_VEC3, &renderer->lightPosition, sizeof(glm::vec3));
	eyePosUniformID = renderables[renderablesCount]->AddUniform("EyePos", ut_VEC3, &renderer->camera.position, sizeof(glm::vec3));
	useLightingUniformID = renderables[renderablesCount]->AddUniform("UseLighting", ut_INT, &useLighting, sizeof(int));
	useNormalMapUniformID = renderables[renderablesCount]->AddUniform("UseNormalMap", ut_INT, &useNormalMap, sizeof(int));
	useShadowsUniformID = renderables[renderablesCount]->AddUniform("UseShadows", ut_INT, &useShadows, sizeof(int));
	meltedUniformID = renderables[renderablesCount]->AddUniform("meltedLevel", ut_FLOAT, &meltedLevel, sizeof(float));
	colorUniformID = renderables[renderablesCount]->AddUniform("color", ut_VEC4, &color, sizeof(glm::vec4));
	renderables[renderablesCount]->AddUniform("M", ut_MAT4_Model, 0, 0);
	renderables[renderablesCount]->AddUniform("V", ut_MAT4_View, 0, 0);
	renderables[renderablesCount]->AddUniform("P", ut_MAT4_Projection, 0, 0);
	renderables[renderablesCount]->layer = rl_TRANSPARENT;

	renderablesCount++;

	renderables[renderablesCount] = renderer->AddRenderableInstance_p(sphereGeoID, glm::translate(pos + glm::vec3(0.0f, 2.0f * scale, 0.0f))* glm::scale(scale * 0.7f, scale * 0.7f, scale * 0.7f), shaderID);
	renderables[renderablesCount]->AddUniform("MVP", ut_MAT4_MVP, 0, 0);
	renderables[renderablesCount]->AddUniform("LightPos", ut_VEC3, &renderer->lightPosition, sizeof(glm::vec3));
	renderables[renderablesCount]->AddUniform("EyePos", ut_VEC3, &renderer->camera.position, sizeof(glm::vec3));
	renderables[renderablesCount]->AddUniform("UseLighting", ut_INT, &useLighting, sizeof(int));
	renderables[renderablesCount]->AddUniform("UseNormalMap", ut_INT, &useNormalMap, sizeof(int));
	renderables[renderablesCount]->AddUniform("UseShadows", ut_INT, &useShadows, sizeof(int));
	renderables[renderablesCount]->AddUniform("meltedLevel", ut_FLOAT, &meltedLevel, sizeof(float));
	renderables[renderablesCount]->AddUniform("color", ut_VEC4, &color, sizeof(glm::vec4));
	renderables[renderablesCount]->AddUniform("M", ut_MAT4_Model, 0, 0);
	renderables[renderablesCount]->AddUniform("V", ut_MAT4_View, 0, 0);
	renderables[renderablesCount]->AddUniform("P", ut_MAT4_Projection, 0, 0);
	renderables[renderablesCount]->layer = rl_TRANSPARENT;

	renderablesCount++;
}

void SceneWindow::CreateNoise()
{
	int width = 128;
	int height = 128;

	noise::module::Perlin perlinNoise;
	perlinNoise.SetSeed(time(NULL));
	perlinNoise.SetFrequency(4.0);
	
	GLubyte* data = new GLubyte[width * height * 6];

	double xRange = 1.0;
	double yRange = 1.0;
	double xFactor = xRange / width;
	double yFactor = yRange / height;

	for(int oct = 0; oct < 6; oct++) 
	{
		perlinNoise.SetOctaveCount(oct + 1);
		for(int i = 0; i < width; i++)
		{
			for(int j = 0; j < height; j++)
			{
				double x = xFactor * i;
				double y = yFactor * j;
				double z = 0.0;

				//float val = (float)perlinNoise.GetValue(x,y,z);
				float val = 0.0f;
				double a,b,c,d;

				a = perlinNoise.GetValue(x, y, z);
				b = perlinNoise.GetValue(x + xRange, y, z);
				c = perlinNoise.GetValue(x, y + yRange, z);
				d = perlinNoise.GetValue(x + xRange, y + yRange, z);

				double xmix = 1.0 - x / xRange;
				double ymix = 1.0 - y / yRange;
				double x1 = glm::mix(a, b, xmix);
				double x2 = glm::mix(c, d, xmix);

				val = glm::mix(x1, x2, ymix);				
				
				val = (val + 1.0f) * 0.5f;
				val = val > 1.0f ? 1.0f : val;
				val = val < 0.0f ? 0.0f : val;

				data[(((j * width) + i) * 4) + oct] = (GLubyte)(val * 255.0f);
			}
		}
	}

	GLuint texID;	
	glGenTextures(1, &texID);
	glActiveTexture(GL_TEXTURE0 + texID);
	
	glBindTexture(GL_TEXTURE_2D, texID);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	delete [] data;

	planeRenderable->AddUniform("noiseMap", ut_TEXTURE, &texID, sizeof(uint));
}

void SceneWindow::Update()
{
	DebugApplication::Update();

	if(!initialized) return;

	useNormalMap = usenormal_b ? 1 : 0;
	useLighting = uselighting_b ? 1 : 0;
	useShadows = useshadows_b ? 1 : 0;

	renderer->CreateShadowMap = useshadows_b;

	planeRenderable->UpdateUniform(lightPosUniformID, &renderer->lightPosition, sizeof(glm::vec3));
	planeRenderable->UpdateUniform(eyePosUniformID, &renderer->camera.position, sizeof(glm::vec3));
	planeRenderable->UpdateUniform(useLightingUniformID, &useLighting, sizeof(int));
	planeRenderable->UpdateUniform(colorUniformID, &color, sizeof(glm::vec4));
	planeRenderable->UpdateUniform(meltedUniformID, &meltedLevel, sizeof(float));
	planeRenderable->UpdateUniform(useLightingUniformID, &useLighting, sizeof(uint));
	planeRenderable->UpdateUniform(useNormalMapUniformID, &useNormalMap, sizeof(uint));
	planeRenderable->UpdateUniform(useShadowsUniformID, &useShadows, sizeof(uint));


	for(int i = 0; i < renderablesCount; i++)
	{
		renderables[i]->UpdateUniform(lightPosUniformID, &renderer->lightPosition, sizeof(glm::vec3));
		renderables[i]->UpdateUniform(eyePosUniformID, &renderer->camera.position, sizeof(glm::vec3));
		renderables[i]->UpdateUniform(useLightingUniformID, &useLighting, sizeof(int));
		renderables[i]->UpdateUniform(colorUniformID, &color, sizeof(glm::vec4));
		renderables[i]->UpdateUniform(meltedUniformID, &meltedLevel, sizeof(float));
		renderables[i]->UpdateUniform(useLightingUniformID, &useLighting, sizeof(uint));
		renderables[i]->UpdateUniform(useNormalMapUniformID, &useNormalMap, sizeof(uint));
		renderables[i]->UpdateUniform(useShadowsUniformID, &useShadows, sizeof(uint));
	}

	DebugShapes::SetShapeTransform(lightShapeID, glm::translate(renderer->lightPosition) * glm::scale(0.2f, 0.2f, 0.2f));
	renderer->lightDirection = glm::normalize(-renderer->lightPosition);
}

#include "SceneWindow_moc.cpp"