#ifndef NORMAL_DISPLAY_H
#define NORMAL_DISPLAY_H

#include <DebugApplication.h>
#include <DebugShapes.h>
#include <Renderer.h>

using namespace DebugTools;

class NormalDisplay : public DebugApplication
{
	Q_OBJECT

private:
	uint shaderIndex;
	Renderer::RenderableInstanceInfo* shapeR;
	uint lightID;

	uint diffValID;
	uint specValID;
	uint lightPosID;
	uint camPosID;
	float diffuseAmount;
	float specAmount;

	glm::vec3 shapePos;
	glm::vec3 lightPos;

	bool shapesCreated;

	bool shapesNormals;
	uint shapesNormalID;
	uint flatNormalID;

public:
	NormalDisplay():shapesCreated(false), shapesNormals(false){}
	~NormalDisplay(){}
	void Initialize();
	void RendererInitialized();

	void CreatePlane();
	void CreateCube();

public slots:
	void Update();
};

#endif // NORMAL_DISPLAY_H