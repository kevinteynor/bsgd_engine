#ifndef RENDER_WIDGET
#define RENDER_WIDGET

#include <GL\glew.h>
#include <QtOpenGL\qglwidget>
#include <glm\glm.hpp>
#include "DebugCamera.h"
#include "ShapeGenerator.h"


class render_widget : public QGLWidget
{
private:
	GLuint indexBufferID;
	GLuint vertexBufferID;

	GLuint programID; 
	GLuint vertexShaderID;
	GLuint fragShaderID;

#pragma region ShapeInfo
	struct GeometryInfo
	{
		GLuint vertexBufferOffset;
		GLuint vertexBufferSize;
		GLuint indexBufferOffset;
		GLuint indexBufferSize;
		GLuint numIndices;
	};

	GeometryInfo GeoInfo_cube;
	GeometryInfo GeoInfo_arrow;
	GeometryInfo GeoInfo_sphere;
	GeometryInfo GeoInfo_torus;
	GeometryInfo GeoInfo_plane;
	GeometryInfo GeoInfo_teapot;
#pragma endregion

	glm::vec2 lastMousePos;

	DebugCamera camera;

protected:
	void initializeGL();
	void shutdown();

	void keyPressEvent(QKeyEvent *e);
	void wheelEvent(QWheelEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);

	void fillBuffer();
	void createShaders();

	void resizeEvent(QResizeEvent* e);

	void paintGL();


public:
	render_widget(void){}
	~render_widget(void);
};

#endif