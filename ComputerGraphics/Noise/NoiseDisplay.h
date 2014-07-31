#ifndef NOISE_DISPLAY_H
#define NOISE_DISPLAY_H

#include <DebugApplication.h>
#include <DebugShapes.h>
#include <Renderer.h>
#include <noise\module\module.h>

using namespace DebugTools;

class NoiseDisplay : public DebugApplication
{
	Q_OBJECT

private:
	Renderer::RenderableInstanceInfo* planeR;
	uint shaderID;

	glm::vec3 color1;
	glm::vec3 color2;

	uint c1ID;
	uint c2ID;

	uint sliceID;

	glm::vec3 scale;
	glm::vec3 offset;

	float low;

	uint lowID;

	bool initialized;

public:
	NoiseDisplay(void) : initialized(false){}
	~NoiseDisplay(void){}

	void Initialize();
	void RendererInitialized();

	void CreatePlane();
	void CreateNoise();

	void Update();
};

#endif //NOISE_DISPLAY_H