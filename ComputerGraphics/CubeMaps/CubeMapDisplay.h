#ifndef CUBE_MAP_DISPLAY
#define CUBE_MAP_DISPLAY

#include <DebugApplication.h>
#include <Renderer.h>

using namespace DebugTools;

class CubeMapDisplay : public DebugApplication
{
	Q_OBJECT

private:
	glm::vec3 spherePos;
	bool shapeCreated;

	uint sphereShaderIndex;
	uint cubeShaderIndex;
	uint lightID;
	Renderer::RenderableInstanceInfo* sphereInfo;
	Renderer::RenderableInstanceInfo* cubeInfo;

	float reflectivity;
	uint reflectivityID;

	uint cubeMapTextureID;
	uint cubeMapUniformID;
	uint eyePosID;
	
	bool drawCubeMap;
	uint drawCubemapUniformID;

public:
	CubeMapDisplay():shapeCreated(false) {}
	~CubeMapDisplay() {}

	void Initialize();
	void RendererInitialized();

	void CreateSphere();
	void CreateCube();

public slots:
	void Update();
};

#endif // CUBE_MAP_DISPLAY