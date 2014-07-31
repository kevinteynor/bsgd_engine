#ifndef CAPTURE_THE_FLAG
#define CAPTURE_THE_FLAG

#include <AStarPath.h>
#include <glm\glm.hpp>
#include <NativeLevel.h>
#include <Qt\qmainwindow.h>
#include <Renderer.h>
#include <Slider.h>
#include <DebugShapes.h>
#include "Player.h"

class DebugCamera;
class QAction;
class QTimer;

class CaptureTheFlag : public QMainWindow
{
	Q_OBJECT

private:
	bool levelLoaded;
	Renderer* renderer;

	QTimer* updateTimer;

public:
	NativeLevel_Static level;
	uint team1_HomeNodeID;
	uint team2_HomeNodeID;

	struct flag
	{
		Player* carrier;
		uint renderableID;
		glm::vec3 position;
		uint homeNodeID;
	} TeamFlags[4];

	static const int NUM_PLAYERS = 4;
	Player** players;

	CaptureTheFlag();
	void Initialize();

	void CaptureFlag(Player* p);
	void ResetFlag(uint teamID);

public slots:
	void Update();
	void OpenLevel();
};


#endif // CAPTURE_THE_FLAG