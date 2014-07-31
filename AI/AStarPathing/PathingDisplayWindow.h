#ifndef PATHING_DISPLAY_WINDOW_H
#define PATHING_DISPLAY_WINDOW_H

#include <AStarPath.h>
#include <glm\glm.hpp>
#include <NativeLevel.h>
#include <Qt\qmainwindow.h>
#include <Qt\qtimer.h>
#include <Renderer.h>
#include <Slider.h>
#include <DebugShapes.h>

class DebugCamera;
class QAction;
class QCheckBox;

class PathingDisplayWindow : public QMainWindow
{
	Q_OBJECT

private:
	bool updateEnabled;
	glm::vec3 camera_position;
	Renderer* renderer;
	QTimer* updateTimer;
	uint shaderID;

	struct NodeRenderableInfo
	{
		uint nodeRenderableID;
		uint numConnections;
		uint* connectionRenderableIDs;

		NodeRenderableInfo() : connectionRenderableIDs(0) {}
		~NodeRenderableInfo()
		{
			connectionRenderableIDs = 0;
		}
	};

	NodeRenderableInfo* pathTreeRenderableInfo;
	NodeRenderableInfo* pathRenderableInfo;

	uint followerID;
	uint currentTargetIndex;
	AStarPath path;

	float followerSpeed;
	Neumont::Slider* followerSpeedSlider;
	QCheckBox* showAllNodes;
	QCheckBox* showCurrentPath;

	QAction* loadLevelAction;
	NativeLevel_Static level;
	
	void UpdatePathRenderableInfo();

public:
	PathingDisplayWindow(){}
	~PathingDisplayWindow(){}

	void Initialize();

public slots:
	void Update();
	void InitializeComponents();
	void OpenLevel();
	void UpdateFollowerSpeed(float value);
	void SetPathNodesVisibility(int state);
	void SetCurrentPathVisibility(int state);
};

#endif // PATHING_DISPLAY_WINDOW_H