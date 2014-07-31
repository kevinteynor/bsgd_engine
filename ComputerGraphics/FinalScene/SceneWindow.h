#pragma once

#include <DebugApplication.h>
#include <DebugShapes.h>
#include <Renderer.h>

using namespace DebugTools;

class SceneWindow : public DebugApplication
{
	Q_OBJECT

private:
	bool initialized;

	uint shaderID;
	uint cubemapShaderID;

	uint cubemapCubeID;
	uint cubemapTextureID;

	uint snowTextureID;
	uint snowNormalID;

	uint lightPosUniformID;
	uint eyePosUniformID;
	uint colorUniformID;
	uint meltedUniformID;

	uint useLightingUniformID;
	uint useNormalMapUniformID;
	uint useShadowsUniformID;
	uint useLighting;
	uint useNormalMap;
	uint useShadows;
	bool uselighting_b;
	bool usenormal_b;
	bool useshadows_b;

	Renderer::RenderableInstanceInfo* cubemapCubeRenderable;
	Renderer::RenderableInstanceInfo* planeRenderable;

	uint lightShapeID;

	Renderer::RenderableInstanceInfo** renderables;
	uint renderablesCount;

	glm::vec4 color;
	float meltedLevel;

	uint sphereGeoID;

public:
	SceneWindow(void);
	~SceneWindow(void);

	void Initialize();
	void RendererInitialized();

	void CreateShapes();
	void CreateSnowman(glm::vec3 pos, float scale);
	void CreateNoise();

	void Update();
};

