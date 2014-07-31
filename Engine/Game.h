#ifndef GAME_H
#define GAME_H

#include "EngineTypeDefs.h"
#include "ExportHeader.h"
#include <Qt\qobject.h>
#include <Qt\qtimer.h>

class ENGINE_SHARED Game : public QObject
{
	Q_OBJECT

private:
	QTimer* gameLoopTimer;

public:
	virtual void Start();

public slots:
	virtual void Update() = 0;
	virtual void Draw() = 0;
};

#endif // GAME_H