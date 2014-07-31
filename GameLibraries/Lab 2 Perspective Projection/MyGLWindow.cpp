#include "MyGLWindow.h"
#include <iostream>
#include <glm\gtx\transform.hpp>
#include <Qt\qfile.h>
#include <Qt\qtextstream.h>
#include <Qt\qdebug.h>
#include <string>
#include <ShapeGenerator.h>
#include <ShapeData.h>
#include "Qt\qevent.h"

//#define JAMIE_CONTROL
#ifndef JAMIE_CONTROL

#define FORWARD Qt::Key_W
#define BACKWARD Qt::Key_S
#define STRAFE_L Qt::Key_Q
#define STRAFE_R Qt::Key_E
#define TURN_L Qt::Key_A
#define TURN_R Qt::Key_D
#define TURN_U Qt::Key_R
#define TURN_D Qt::Key_F
#define UP Qt::Key_X
#define DOWN Qt::Key_Z

#define MOUSE_TRACKING false

#else

#define FORWARD Qt::Key_W
#define BACKWARD Qt::Key_S
#define STRAFE_L Qt::Key_A
#define STRAFE_R Qt::Key_D
#define TURN_L Qt::Key_Q
#define TURN_R Qt::Key_E
#define TURN_U Qt::Key_R
#define TURN_D Qt::Key_F
#define UP Qt::Key_R
#define DOWN Qt::Key_F

#define MOUSE_TRACKING true

#endif // JAMIE_CONTROLS


void MyGLWidget::initializeGL()
{
	glewInit();

	setMouseTracking(MOUSE_TRACKING);
	//showFullScreen();
	//glViewport(0,0, 1920, 1080);

	//camera.SetPosition(0,2,-4);
	lastMousePos = glm::vec2(-1,-1);
	
	glEnable(GL_DEPTH_TEST);

	glClearColor(0,0,0,1);
	fillBuffer();
	createShaders();
}
void MyGLWidget::shutdown()
{
	glDeleteBuffers(1, &arrow_vertexBufferID);
	glDeleteBuffers(2, &torus_vertexBufferID);
}

void MyGLWidget::updateGL()
{

}

void MyGLWidget::keyPressEvent(QKeyEvent *e)
{
	if(e->key() == Qt::Key_Escape)
		exit(0);

	// FORWARD
	if(e->key() == FORWARD)
		camera.Move_Forward(0.05f);
	if(e->key() == BACKWARD)
		camera.Move_Forward(-0.05f);
	
	// ROTATE-HORIZONTAL
	if(e->key() == TURN_L)
		camera.Rotate_Horizontal(5.0f);
	if(e->key() == TURN_R)
		camera.Rotate_Horizontal(-5.0f);

	// STRAFE
	if(e->key() == STRAFE_L)
		camera.Move_Sideways(0.05f);
	if(e->key() == STRAFE_R)
		camera.Move_Sideways(-0.05f);

	// UP DOWN
	if(e->key() == UP)
		camera.Move_Vertical(0.05f);
	if(e->key() == DOWN)
		camera.Move_Vertical(-0.05f);

	// ROTATE-VERTICAL
	if(e->key() == TURN_U)
		camera.Rotate_Vertical(5.0f);
	if(e->key() == TURN_D)
		camera.Rotate_Vertical(-5.0f);

	glDraw();
}
void MyGLWidget::wheelEvent(QWheelEvent *e)
{
	camera.Move_Forward((float)(e->delta() * 0.01f));

	glDraw();
}
void MyGLWidget::mousePressEvent(QMouseEvent *e)
{
	lastMousePos = glm::vec2(e->x(), e->y());
}
void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
	glm::vec2 newPos = glm::vec2(e->x(), e->y());

	if(glm::distance(newPos, lastMousePos) > 100)
		lastMousePos = newPos;

	camera.Rotate_Horizontal((lastMousePos.x - newPos.x) * 0.4f);
	camera.Rotate_Vertical((lastMousePos.y - newPos.y) * 0.4f);

	lastMousePos = newPos;

	glDraw();
}

void MyGLWidget::fillBuffer()
{

// arrow
	Neumont::ShapeData data = Neumont::ShapeGenerator::makeArrow();
	arrow_numIndices = data.numIndices;
	arrow_shapeData = data;

	glGenBuffers(1, &arrow_vertexBufferID);
	glGenBuffers(1, &arrow_indexBufferID);

	glBindBuffer(GL_ARRAY_BUFFER, arrow_vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, data.vertexBufferSize(), data.verts, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, arrow_indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indexBufferSize(), data.indices, GL_STATIC_DRAW);

	data.cleanUp();

// torus
	data = Neumont::ShapeGenerator::makeTorus(50);
	torus_numIndices = data.numIndices;
	torus_shapeData = data;

	glGenBuffers(1, &torus_vertexBufferID);
	glGenBuffers(1, &torus_indexBufferID);

	glBindBuffer(GL_ARRAY_BUFFER, torus_vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, data.vertexBufferSize(), data.verts, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, torus_indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indexBufferSize(), data.indices, GL_STATIC_DRAW);

	data.cleanUp();

// Tell GL how to read the bits (3 for the position, 4 for the color)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, BUFFER_OFFSET(Neumont::Vertex::COLOR_OFFSET));
}
void MyGLWidget::createShaders()
{
	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	programID = glCreateProgram();

	compileShader("MyVertexShader.sh", vertexShaderID);
	compileShader("MyFragmentShader.sh", fragShaderID);
	
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragShaderID);

	glLinkProgram(programID);

	glUseProgram(programID);
}
void MyGLWidget::compileShader(const char* fileName, GLuint shaderID)
{
	QFile input(fileName);
	if(! input.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "File failed to open: " << fileName;
		exit(1);
	}
	QTextStream stream(&input);
	QString shaderCodeText = stream.readAll();
	input.close();

	std::string temp = shaderCodeText.toStdString();
	const char* shaderSource = temp.c_str();

	const char* buf[1];
	buf[0] = shaderSource;
	glShaderSource(shaderID, 1, buf, NULL);
	glCompileShader(shaderID);

	GLint status;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
	if(status != GL_TRUE)
	{
		GLint infoLogLen;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLen);

		char* buffer = new char[infoLogLen];
		GLsizei bitBucket;
		glGetShaderInfoLog(shaderID, infoLogLen, &bitBucket, buffer);

		std::cout << buffer << std::endl;

		delete[] buffer;
		//exit(1);
	}
}

void MyGLWidget::resizeEvent(QResizeEvent *e)
{
	glViewport(0,0, width(), height());
}

void MyGLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::mat4 cameraView = camera.GetView();
	glm::mat4 projection = glm::perspective(
		60.0f, 
		((float)width()) / height(),
		0.1f, 100.0f);

// arrow
	glBindBuffer(GL_ARRAY_BUFFER, arrow_vertexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, arrow_indexBufferID);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, BUFFER_OFFSET(Neumont::Vertex::COLOR_OFFSET));

	glm::mat4 model;

	glm::mat4 fullTransform = projection * cameraView * model;
	GLint transformUniformLocation = glGetUniformLocation(programID, "transform");
	glUniformMatrix4fv(transformUniformLocation, 1, GL_FALSE, &fullTransform[0][0]);
	glDrawElements(GL_TRIANGLES, arrow_numIndices, GL_UNSIGNED_SHORT, NULL);

	for(int i = 0; i < 1000; i++)
	{
		fullTransform = projection * cameraView * (model * glm::translate(glm::vec3(i * 2.0f, 0.0f, 0.0f)));	
		transformUniformLocation = glGetUniformLocation(programID, "transform");
		glUniformMatrix4fv(transformUniformLocation, 1, GL_FALSE, &fullTransform[0][0]);
		glDrawElements(GL_TRIANGLES, arrow_numIndices, GL_UNSIGNED_SHORT, NULL);
	}

// torus
	glBindBuffer(GL_ARRAY_BUFFER, torus_vertexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, torus_indexBufferID);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, BUFFER_OFFSET(Neumont::Vertex::COLOR_OFFSET));

	model = model * glm::translate(glm::vec3(-3,1,0));

	fullTransform = projection * cameraView * model;
	transformUniformLocation = glGetUniformLocation(programID, "transform");
	glUniformMatrix4fv(transformUniformLocation, 1, GL_FALSE, &fullTransform[0][0]);
	glDrawElements(GL_TRIANGLES, torus_numIndices, GL_UNSIGNED_SHORT, NULL);

}