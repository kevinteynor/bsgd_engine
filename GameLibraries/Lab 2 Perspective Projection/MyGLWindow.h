#ifndef MY_GL_WINDOW
#define MY_GL_WINDOW

#include <GL\glew.h>
#include <QtOpenGL\qglwidget>
#include "ShapeGenerator.h"
#include <glm\glm.hpp>
#include "DebugCamera.h"

class MyGLWidget : public QGLWidget
{
private:
	GLuint arrow_vertexBufferID;
	GLuint arrow_indexBufferID;
	GLuint arrow_numIndices;

	GLuint torus_vertexBufferID;
	GLuint torus_indexBufferID;
	GLuint torus_numIndices;

	GLuint vertexShaderID;
	GLuint fragShaderID;
	GLuint programID;

	glm::vec2 lastMousePos;

	Neumont::ShapeData arrow_shapeData;
	Neumont::ShapeData torus_shapeData;

	DebugCamera camera;

	void compileShader(const char* fileName, GLuint shaderID);

protected:
	void initializeGL();
	void shutdown();
	
	void keyPressEvent(QKeyEvent *e);
	void wheelEvent(QWheelEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);

	void fillBuffer();
	void createShaders();

	void MyGLWidget::resizeEvent(QResizeEvent *e);

	void paintGL();

public:
	void updateGL();

};

#endif // MY_GL_WINDOW