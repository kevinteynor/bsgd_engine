#ifndef PARENT_WINDOW_H
#define PARENT_WINDOW_H

#include <Qt\qobject.h>
#include "Renderer.h"
#include "TypeDefs.h"

class ParentWindow : public QObject
{
	Q_OBJECT

private:
	uint cubeGeometryID, sphereGeometryID, arrowGeometryID, torusGeometryID;
	uint shader1ID, shader2ID, shader3ID, shader4ID;
	Renderer* renderer;

public:
	ParentWindow(void);
	~ParentWindow(void);

	void CreateShapes();
	void InitializeRenderer();

public slots:
	void InitializeComponents();
	// the handler
	// definitions for the functions to be called upon an event
};

#endif // PARENT_WINDOW_H