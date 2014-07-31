#ifndef GRID_WIDGET_2D
#define GRID_WIDGET_2D

#include <glm\glm.hpp>
#include <Qt\qobject.h>
#include <Qt\qbrush.h>
#include <Qt\qpen.h>
#include <QtGui\qwidget.h>
#include "EngineTypeDefs.h"

struct PointWatchInfo
{
	float* x;
	float* y;
	const float* inverseMass;
	bool visible;
	bool isCircle;
	QBrush color;

	~PointWatchInfo() { x = 0; y = 0; inverseMass = 0; }
};
struct AttachedVectorWatchInfo
{
	const float* x;
	const float* y;
	const float* x_origin;
	const float* y_origin;
	bool visible;
	QPen color;

	~AttachedVectorWatchInfo() { x = 0; y = 0; x_origin = 0; y_origin = 0; }
};

class GridWidget2D : public QWidget
{
	Q_OBJECT

private:
	static const uint MAX_WATCHES = 64;

	uint watchedPoints;
	PointWatchInfo PointWatchInfos[MAX_WATCHES];

	uint watchedAttachedVectors;
	AttachedVectorWatchInfo AttachedVectorWatchInfos[MAX_WATCHES];

	
	glm::vec2 lastMousePos;

	PointWatchInfo* GetClosestPointWatchInfo(glm::vec2 p);

public:
	GridWidget2D(void);
	~GridWidget2D(void);

	bool ClickAndDrag;
	PointWatchInfo* draggingPoint;

	glm::vec2 gridOriginOffset;
	int gridLineSpacing;

	void paintEvent(QPaintEvent*);

	PointWatchInfo* WatchPoint(float* x, float* y, const float* imass, Qt::GlobalColor color, bool circle = true);
	AttachedVectorWatchInfo* WatchAttachedVector(const float* x, const float* y, const float* x_origin, const float* y_origin, Qt::GlobalColor color);

protected:
	void wheelEvent(QWheelEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
};

#endif // GRID_WIDGET_2D