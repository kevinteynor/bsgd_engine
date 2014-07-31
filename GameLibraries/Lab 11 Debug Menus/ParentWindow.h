#ifndef PARENT_WINDOW_H
#define PARENT_WINDOW_H

#include <Qt\qobject.h>
#include <TypeDefs.h>
#include <UpdateClock.h>
#include <QtGui\qwidget.h>

class Renderer;
class QTimer;
class DebugCamera;

class ParentWindow : public QObject
{
	Q_OBJECT

private:
	uint cubeGeometryID, shaderID;

	Renderer* renderer;
	QWidget* mainWindow;
	QTimer* updateTimer;
	DebugCamera* playerCamera;
	DebugCamera* debugCamera;

	void InitializeRenderer();

public:
	ParentWindow(void);
	~ParentWindow(void);

	void Initialize();
	void Shutdown();

public slots:
	void InitializeComponents();
	void Update();

};

#endif PARENT_WINDOW_H