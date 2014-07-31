#include "GridWidget_2D.h"
#include <Qt\qpainter.h>
#include <Qt\qevent.h>
#include <climits>
#include <iostream>

#define MIN_LINE_SPACING 4
#define MAX_LINE_SPACING 2500
#define POINT_RADIUS_SCALE 3.0f

GridWidget_2D::GridWidget_2D(void)
{
	ClickAndDrag = false;
	watchedPoints = 0;
	watchedAttachedVectors = 0;
	gridLineSpacing = 20;
	gridOriginOffset = glm::vec2(width() / 2.0f, height() / 2.0f);
}
GridWidget_2D::~GridWidget_2D(void)
{
#pragma warning( disable : 4154 )
	// HELP
	//delete [] PointWatchInfos;
}

void GridWidget_2D::paintEvent(QPaintEvent* e)
{
	e;

	QPainter painter(this);
	int w = width();
	int h = height();
	
#pragma region BACKGROUND
	// draw background
	painter.fillRect(0,0, w, h, Qt::white);

	// draw grid (around origin)
	int x = gridOriginOffset.x;
	while(x > 0)
	{
		x -= gridLineSpacing;
		painter.drawLine(x, 0, x, h);
	}
	x = gridOriginOffset.x;
	while(x < w)
	{
		x += gridLineSpacing;
		painter.drawLine(x, 0, x, h);
	}
	int y = gridOriginOffset.y;
	while(y > 0)
	{
		y -= gridLineSpacing;
		painter.drawLine(0, y, w, y);
	}
	y = gridOriginOffset.y;
	while(y < h)
	{
		y += gridLineSpacing;
		painter.drawLine(0, y, w, y);
	}

	// horizontal axis
	painter.setPen(QPen(Qt::red));
	painter.drawLine(0, gridOriginOffset.y, w, gridOriginOffset.y);
	// vertical axis
	painter.setPen(QPen(Qt::green));
	painter.drawLine(gridOriginOffset.x, 0, gridOriginOffset.x, h);

	// Border
	painter.setPen(QPen(Qt::black));
	painter.drawLine(0, 0, w, 0);
	painter.drawLine(0, 0, 0, h);
	painter.drawLine(w - 1, h - 1, w - 1, 0);
	painter.drawLine(w - 1, h - 1, 0, h - 1);

#pragma endregion
	painter.setPen(QPen(Qt::black));
#pragma region POINTS
	
	// draw points
	for(uint i = 0; i < watchedPoints; i++)
	{
		PointWatchInfo v = PointWatchInfos[i];
		if(!v.visible) continue;

		float radius = 3;
		if(v.inverseMass != 0)
			radius = 1 / *v.inverseMass / POINT_RADIUS_SCALE;

		painter.setBrush(v.color);
		if(v.isCircle)
		{
			painter.drawEllipse(
				QPointF(
					(*v.x * gridLineSpacing) + gridOriginOffset.x,
					-(*v.y * gridLineSpacing) + gridOriginOffset.y),
				radius * gridLineSpacing, radius * gridLineSpacing);
		}
		else 
		{
			painter.drawRect(
				(*v.x * gridLineSpacing) + gridOriginOffset.x - (radius * gridLineSpacing),
				-(*v.y * gridLineSpacing) + gridOriginOffset.y - (radius * gridLineSpacing),
				radius * gridLineSpacing * 2, radius * gridLineSpacing * 2);
		}
	}
#pragma endregion

#pragma region VECTORS
	// draw attached vectors
	for(uint i = 0; i < watchedAttachedVectors; i++)
	{
		AttachedVectorWatchInfo v = AttachedVectorWatchInfos[i];
		if(!v.visible) continue;

		painter.setPen(v.color);
		QPointF origin = QPointF(
			(*v.x_origin * gridLineSpacing) + gridOriginOffset.x,
			-(*v.y_origin * gridLineSpacing) + gridOriginOffset.y);
		QPointF tip = QPointF(
			((*v.x_origin + *v.x) * gridLineSpacing) + gridOriginOffset.x,
			-((*v.y_origin + *v.y) * gridLineSpacing) + gridOriginOffset.y);

		painter.drawLine(origin, tip);

		float angle = atan2f(tip.y() - origin.y(), tip.x() - origin.x());
		const float EDGE_LEN = 10;
		const float OFFSET_ANGLE = 3 * (PI / 4);
		float leftEdgeAngle = angle + OFFSET_ANGLE;
		float rightEdgeAngle = angle - OFFSET_ANGLE;
					
		QPointF arrow = tip + QPointF(cosf(leftEdgeAngle) * EDGE_LEN, sinf(leftEdgeAngle) * EDGE_LEN);
		painter.drawLine(tip, arrow);
		
		arrow = arrow = tip + QPointF(cosf(rightEdgeAngle) * EDGE_LEN, sinf(rightEdgeAngle) * EDGE_LEN);
		painter.drawLine(tip, arrow);
	}
#pragma endregion
}

PointWatchInfo* GridWidget_2D::WatchPoint(float* x, float* y, const float* imass, Qt::GlobalColor color, bool circle)
{
	if(watchedPoints >= MAX_WATCHES)
		return 0;

	PointWatchInfo& info = PointWatchInfos[watchedPoints++];
	info.x = x;
	info.y = y;
	info.isCircle = circle;
	info.inverseMass = imass;
	info.color = QBrush(color);

	return &info;
}
AttachedVectorWatchInfo* GridWidget_2D::WatchAttachedVector(const float* x, const float* y, const float* x_origin, const float* y_origin, Qt::GlobalColor color)
{
	if(watchedPoints >= MAX_WATCHES)
		return 0;

	AttachedVectorWatchInfo& info = AttachedVectorWatchInfos[watchedAttachedVectors++];
	info.x = x;
	info.y = y;
	info.x_origin = x_origin;
	info.y_origin = y_origin;
	info.color = QPen(color);
	info.color.setWidthF(2);

	return &info;
}

void GridWidget_2D::wheelEvent(QWheelEvent *e)
{
	float d = e->delta() * gridLineSpacing * 0.001f;

	if(d < 0 && d > -1)
		d = -1;
	if(d > 0 && d < 1)
		d = 1;

	gridLineSpacing += (int)(d);
	
	if(gridLineSpacing < MIN_LINE_SPACING) gridLineSpacing = MIN_LINE_SPACING;
	if(gridLineSpacing > MAX_LINE_SPACING) gridLineSpacing = MAX_LINE_SPACING;
}
void GridWidget_2D::mousePressEvent(QMouseEvent *e)
{
	lastMousePos = glm::vec2(e->x(), e->y());

	if(ClickAndDrag)
	{
		draggingPoint = GetClosestPointWatchInfo(glm::vec2(e->x(), e->y()));

		if(!draggingPoint) return;

		glm::vec2 mousePosInGridCoords = glm::vec2(
			(e->x() - gridOriginOffset.x) / gridLineSpacing,
			-(e->y() - gridOriginOffset.y) / gridLineSpacing);

		*(draggingPoint->x) = mousePosInGridCoords.x;
		*(draggingPoint->y) = mousePosInGridCoords.y;
	}
}
void GridWidget_2D::mouseMoveEvent(QMouseEvent *e)
{
	glm::vec2 newPos = glm::vec2(e->x(), e->y());

	glm::vec2 diff = (newPos - lastMousePos);
	
	if(ClickAndDrag && draggingPoint)
	{
		glm::vec2 mousePosInGridCoords = glm::vec2(
			(newPos.x - gridOriginOffset.x) / gridLineSpacing,
			-(newPos.y - gridOriginOffset.y) / gridLineSpacing);

		*(draggingPoint->x) = mousePosInGridCoords.x;
		*(draggingPoint->y) = mousePosInGridCoords.y;
	}
	else
	{
		gridOriginOffset += diff;
	}

	lastMousePos = newPos;

	
}
void GridWidget_2D::mouseReleaseEvent(QMouseEvent *e)
{
	e;
	if(ClickAndDrag && draggingPoint)
		draggingPoint = 0;
}

PointWatchInfo* GridWidget_2D::GetClosestPointWatchInfo(glm::vec2 p)
{
	p -= gridOriginOffset;
	p /= gridLineSpacing;
	p = glm::vec2(p.x, -p.y);

	float minDist = gridLineSpacing / 10.0f;
	PointWatchInfo* closest = 0;

	for(uint i = 0; i < watchedPoints; i++)
	{
		if(glm::length(p - glm::vec2(*(PointWatchInfos[i].x), *(PointWatchInfos[i].y))) < minDist)
		{
			closest = &PointWatchInfos[i];
			minDist = glm::length(p - glm::vec2(*(PointWatchInfos[i].x), *(PointWatchInfos[i].y)));
		}
	}

	return closest;
}