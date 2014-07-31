#ifndef PARENT_WINDOW_H
#define PARENT_WINDOW_H

#include <Qt\qobject.h>
#include <Qt\qtimer.h>
#include "TypeDefs.h"
#include "Renderer.h"
#include "DebugCamera.h"
#include "UpdateClock.h"

class ParentWindow : public QObject
{
	Q_OBJECT

private:
	uint cubeGeometryID, sphereGeometryID, arrowGeometryID, torusGeometryID;
	uint shader1ID, shader2ID, shader3ID, shader4ID;

	Renderer::RenderableInstanceInfo* cubeInfo;
	Renderer::RenderableInstanceInfo* cubeInfo2;
	Renderer::RenderableInstanceInfo* sphereInfo;

	uint basicShaderInfo;

	Renderer* renderer;
	DebugCamera camera;
	QTimer* updateTimer;

public:
	ParentWindow(void);
	~ParentWindow(void);

	void CreateShapes();
	void InitializeRenderer();

public slots:
	void InitializeComponents();
	void UpdateUniforms();
	void Update();
	// the handler
	// definitions for the functions to be called upon an event
};

#endif // PARENT_WINDOW_H