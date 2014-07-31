#include <GL\glew.h>

#include "NoiseDisplay.h"

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <ShapeData.h>
#include <ShapeGenerator.h>
#include <noise\noise.h>

#include <Qt\qdebug.h>
#include <time.h>

using namespace noise::module;

void NoiseDisplay::Initialize()
{
	DebugApplication::Initialize();
}
void NoiseDisplay::RendererInitialized()
{
	DebugApplication::RendererInitialized();

	CreatePlane();
	CreateNoise();
}

void NoiseDisplay::CreatePlane()
{
	//shaderID = renderer->AddShader("vertex.sh", "fragment.sh");
	shaderID = renderer->AddShader("woodVertex.sh", "woodFragment.sh");

	//Neumont::ShapeData data = Neumont::ShapeGenerator::makePlane(10,10);
	Neumont::ShapeData data = Neumont::ShapeGenerator::makeTeapot(14, glm::mat4());
	uint planeGeo = renderer->AddGeometry(data.verts, data.numVerts, data.indices, data.numIndices, rm_TRIANGLES);
	planeR = renderer->AddRenderableInstance_p(planeGeo, glm::translate(0.0f,-2.0f,0.0f) * glm::rotate(-90.0f, 1.0f, 0.0f,0.0f), shaderID);
	planeR->AddUniform("MVP", ut_MAT4_MVP, 0, 0);

	scale = glm::vec3(10.0f, 20.0f, 1.0f);
	offset = glm::vec3(-0.5f, -0.5f, -0.5f);

	glm::mat4 slice = glm::translate(offset) * glm::scale(scale);
	sliceID = planeR->AddUniform("Slice", ut_MAT4, &slice, sizeof(glm::mat4));

	color1 = glm::vec3(0.5f, 0.3f, 0.15f);
	color2 = glm::vec3(0.7f, 0.55f, 0.3f);

	c1ID = planeR->AddUniform("color1", ut_VEC3, &color1, sizeof(glm::vec3));
	c2ID = planeR->AddUniform("color2", ut_VEC3, &color1, sizeof(glm::vec3));

	low = 0.1f;
	lowID = planeR->AddUniform("LowThreshold", ut_FLOAT, &low, sizeof(float));

	DebugWidget::WatchFloatSlider("c1.r", &color1.r, 0, 1);
	DebugWidget::WatchFloatSlider("c1.g", &color1.g, 0, 1);
	DebugWidget::WatchFloatSlider("c1.b", &color1.b, 0, 1);

	DebugWidget::WatchFloatSlider("c2.r", &color2.r, 0, 1);
	DebugWidget::WatchFloatSlider("c2.g", &color2.g, 0, 1);
	DebugWidget::WatchFloatSlider("c2.b", &color2.b, 0, 1);

	DebugWidget::WatchFloatSlider("scale", &scale.x, 0, 50);
	DebugWidget::WatchFloatSlider("offset", &offset.x, -20, 20);

	DebugWidget::WatchFloatSlider("Low", &low, 0, 1);

	initialized = true;
}

void NoiseDisplay::CreateNoise()
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

	glActiveTexture(GL_TEXTURE3);

	GLuint texID;
	
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	delete [] data;
	uint activeTexture = 3;
	planeR->AddUniform("NoiseTex", ut_TEXTURE, &activeTexture, sizeof(uint));
}

void NoiseDisplay::Update()
{
	DebugApplication::Update();

	if(!initialized) return;

	planeR->UpdateUniform(c1ID, &color1, sizeof(glm::vec3));
	planeR->UpdateUniform(c2ID, &color2, sizeof(glm::vec3));
	planeR->UpdateUniform(sliceID, &(glm::translate(offset) * glm::scale(scale)), sizeof(glm::mat4));

	planeR->UpdateUniform(lowID, &low, sizeof(float));
}

#include "NoiseDisplay_moc.cpp"