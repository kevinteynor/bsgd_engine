#include "AlphaDisplay.h"

#include <DebugApplication.h>
#include <DebugShapes.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <Qt\qdockwidget.h>
#include <Qt\qboxlayout.h>
#include <Qt\qmainwindow.h>

#include <Qt\qdebug.h>

using namespace DebugTools;

void AlphaDisplay::Initialize()
{
	DebugApplication::Initialize();
	cubePos = glm::vec3(0,0,-2);
	arrowPos = glm::vec3(0,0,0);
	cubeColor = glm::vec4(1.0f, 0, 0, 0.5f);
	arrowColor = glm::vec4(0, 0, 1.0f, 1);

	DebugWidget::WatchVec3("CubePos:", &cubePos.x, &cubePos.y, &cubePos.z);
	DebugWidget::WatchVec3("ArrowPos:", &arrowPos.x, &arrowPos.y, &arrowPos.z);

	DebugWidget::WatchFloatSlider("Cube Alpha:", &cubeColor.a, 0, 1);
}
void AlphaDisplay::RendererInitialized()
{
	cubeID = DebugShapes::MakeCube(glm::translate(cubePos), cubeColor, rl_TRANSPARENT);
	arrowID = DebugShapes::MakeArrow(glm::translate(arrowPos), arrowColor, rl_OPAQUE);

	DebugApplication::RendererInitialized();
}

void AlphaDisplay::Update()
{
	DebugApplication::Update();

	DebugShapes::SetShapeColor(cubeID, cubeColor);
	DebugShapes::SetShapeColor(arrowID, arrowColor);
}

void AlphaDisplay::ChangeArrowColor()
{
	// NOT GETTING CALLED D:

	DebugShapes::SetShapeColor(arrowID, glm::vec4(0,1.0f,0,0.5f));
	qDebug() << "color change called";
}

#include "AlphaDisplay_moc.cpp"