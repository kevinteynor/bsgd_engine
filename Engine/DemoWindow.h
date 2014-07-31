#ifndef DEMO_WINDOW_H
#define DEMO_WINDOW_H

#include <glm\glm.hpp>
#include <Qt\qobject.h>
#include <QtGui\qwidget.h>

#include "ExportHeader.h"
#include "TypeDefs.h"

class GridWidget2D;
struct PointWatchInfo;
struct AttachedVectorWatchInfo;
class QTimer;
class QHBoxLayout;
class QVBoxLayout;

class ENGINE_SHARED DemoWindow : public QWidget
{
	Q_OBJECT

private:
	QWidget* mainWindow;
	QTimer* updateTimer;

	QVBoxLayout* mainLayout;
	QVBoxLayout* debugLayout;

	virtual void InitializeDebugGui() = 0;

public:
	DemoWindow();
	~DemoWindow();

	void Initialize();
	PointWatchInfo* WatchVector(glm::vec2*, Qt::GlobalColor);
	PointWatchInfo* WatchPoint(glm::vec2*, const float* iMass, Qt::GlobalColor, bool circle = true);
	AttachedVectorWatchInfo* WatchVector(const glm::vec2*, const glm::vec2*, Qt::GlobalColor);

protected:
	GridWidget2D* gridDisplay;

public slots:
	virtual void Update();

};

#endif // DEMO_WINDOW_H