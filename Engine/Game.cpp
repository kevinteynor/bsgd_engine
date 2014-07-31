#include "Game.h"
#include "UpdateClock.h"
#include <Qt\qtimer.h>
#include <Qt\qobject.h>

void Game::Start()
{
	gameLoopTimer = new QTimer();

	connect(gameLoopTimer, SIGNAL(timeout()), this, SLOT(Update()));
	connect(gameLoopTimer, SIGNAL(timeout()), this, SLOT(Draw()));

	gameLoopTimer->start(16);	
}