#ifndef ALPHA_DISPLAY_H
#define ALPHA_DISPLAY_H

#include <DebugApplication.h>
#include <DebugShapes.h>
#include <glm\glm.hpp>
#include <Qt\qmainwindow.h>
#include <Renderer.h>

using namespace DebugTools;

// http://stackoverflow.com/questions/3388294/opengl-question-about-the-usage-of-gldepthmask

class AlphaDisplay : public DebugApplication
{
	Q_OBJECT

private:
	uint arrowID;
	uint cubeID;

	glm::vec3 arrowPos;
	glm::vec3 cubePos;

	glm::vec4 arrowColor;
	glm::vec4 cubeColor;

public:
	AlphaDisplay(){}
	~AlphaDisplay(){}
	void Initialize();
	void RendererInitialized();

	void ChangeArrowColor();

public slots:
	void Update();
};

#endif // ALPHA_DISPLAY_H