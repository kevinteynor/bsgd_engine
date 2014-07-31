#ifndef SHADOW_DISPLAY_H
#define SHADOW_DISPLAY_H

#include <DebugApplication.h>
#include <DebugShapes.h>
#include <Renderer.h>

using namespace DebugTools;

class ShadowDisplay : public DebugApplication
{
	Q_OBJECT

private:
	uint cubeID;
	uint sphereID;
	uint arrowID;
	uint planeID;
	uint lightID;

	uint shaderID;

	Renderer::RenderableInstanceInfo* sphereR;
	Renderer::RenderableInstanceInfo* cubeR;
	Renderer::RenderableInstanceInfo* arrowR;
	Renderer::RenderableInstanceInfo* planeR;

	glm::vec3 cubePos;
	glm::vec3 spherePos;
	glm::vec3 arrowPos;
	glm::vec3 planePos;
	glm::vec3 lightPos;

public:
	ShadowDisplay(){}
	~ShadowDisplay(){}
	void Initialize();
	void RendererInitialized();

	void CreateShapes();

public slots:
	void Update();

};

#endif // SHADOW_DISPLAY_H