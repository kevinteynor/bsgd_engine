#include "render_widget.h"
#include <glm\gtx\transform.hpp>
#include <ShapeGenerator.h>
#include <ShapeData.h>
#include "ShaderManager.h"
#include <Qt\qevent.h>
#include <Qt\qtextstream.h>
#include <Qt\qdebug.h>
#include <string>

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


void render_widget::initializeGL()
{
	glewInit();
	setMinimumHeight(900);

	camera.SetPosition(0,3,-10);
	camera.SetTarget(0,-3,10);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0,0,0,1);

	fillBuffer();
	createShaders();

	glUseProgram(programID);
}

render_widget::~render_widget()
{
	shutdown();
}
void render_widget::shutdown()
{
	glDeleteBuffers(1, &indexBufferID);
	glDeleteBuffers(1, &vertexBufferID);
}

void render_widget::keyPressEvent(QKeyEvent *e)
{
	qDebug() << "keypressed";

	if(e->key() == Qt::Key_Escape)
		exit(0);

	// FORWARD
	if(e->key() == FORWARD)
		camera.Move_Forward(0.25f);
	if(e->key() == BACKWARD)
		camera.Move_Forward(-0.25f);
	
	// ROTATE-HORIZONTAL
	if(e->key() == TURN_L)
		camera.Rotate_Horizontal(5.0f);
	if(e->key() == TURN_R)
		camera.Rotate_Horizontal(-5.0f);

	// STRAFE
	if(e->key() == STRAFE_L)
		camera.Move_Sideways(0.25f);
	if(e->key() == STRAFE_R)
		camera.Move_Sideways(-0.25f);

	// UP DOWN
	if(e->key() == UP)
		camera.Move_Vertical(0.25f);
	if(e->key() == DOWN)
		camera.Move_Vertical(-0.25f);

	// ROTATE-VERTICAL
	if(e->key() == TURN_U)
		camera.Rotate_Vertical(5.0f);
	if(e->key() == TURN_D)
		camera.Rotate_Vertical(-5.0f);

	qDebug() << camera.direction.x << "," << camera.direction.y << "," << camera.direction.z;
	glDraw();
}
void render_widget::wheelEvent(QWheelEvent *e)
{
	camera.Move_Forward((float)(e->delta() * 0.005f));

	glDraw();
}
void render_widget::mousePressEvent(QMouseEvent *e)
{
	lastMousePos = glm::vec2(e->x(), e->y());
}
void render_widget::mouseMoveEvent(QMouseEvent *e)
{
	glm::vec2 newPos = glm::vec2(e->x(), e->y());

	if(glm::distance(newPos, lastMousePos) > 100)
		lastMousePos = newPos;

	camera.Rotate_Horizontal((lastMousePos.x - newPos.x) * 0.4f);
	camera.Rotate_Vertical((lastMousePos.y - newPos.y) * 0.4f);

	lastMousePos = newPos;

	glDraw();
}

void render_widget::fillBuffer()
{
	GLuint currentOffset_vertex = 0;
	GLuint currentOffset_index = 0; 
		 
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, 1048576, NULL, GL_STATIC_DRAW);

	glGenBuffers(1, &indexBufferID); 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID); 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 1048576, NULL, GL_STATIC_DRAW);

#pragma region CUBE
	Neumont::ShapeData shapeData = Neumont::ShapeGenerator::makeCube();
	GeoInfo_cube.numIndices = shapeData.numIndices;
	GeoInfo_cube.vertexBufferSize = shapeData.vertexBufferSize();
	GeoInfo_cube.vertexBufferOffset = currentOffset_vertex;
	GeoInfo_cube.indexBufferSize = shapeData.indexBufferSize(); 
	GeoInfo_cube.indexBufferOffset = currentOffset_index; 

	currentOffset_vertex += shapeData.vertexBufferSize();
	currentOffset_index += shapeData.indexBufferSize();

	glBufferSubData(GL_ARRAY_BUFFER, GeoInfo_cube.vertexBufferOffset, GeoInfo_cube.vertexBufferSize, shapeData.verts);	
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, GeoInfo_cube.indexBufferOffset, GeoInfo_cube.indexBufferSize, shapeData.indices);
#pragma endregion
#pragma region ARROW
	shapeData = Neumont::ShapeGenerator::makeArrow();
	GeoInfo_arrow.numIndices = shapeData.numIndices;
	GeoInfo_arrow.vertexBufferSize = shapeData.vertexBufferSize();
	GeoInfo_arrow.vertexBufferOffset = currentOffset_vertex;
	GeoInfo_arrow.indexBufferSize = shapeData.indexBufferSize(); 
	GeoInfo_arrow.indexBufferOffset = currentOffset_index; 

	currentOffset_vertex += shapeData.vertexBufferSize();
	currentOffset_index += shapeData.indexBufferSize();

	glBufferSubData(GL_ARRAY_BUFFER, GeoInfo_arrow.vertexBufferOffset, GeoInfo_arrow.vertexBufferSize, shapeData.verts);	
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, GeoInfo_arrow.indexBufferOffset, GeoInfo_arrow.indexBufferSize, shapeData.indices);
#pragma endregion
#pragma region SPHERE
	shapeData = Neumont::ShapeGenerator::makeSphere(30);
	GeoInfo_sphere.numIndices = shapeData.numIndices;
	GeoInfo_sphere.vertexBufferSize = shapeData.vertexBufferSize();
	GeoInfo_sphere.vertexBufferOffset = currentOffset_vertex;
	GeoInfo_sphere.indexBufferSize = shapeData.indexBufferSize(); 
	GeoInfo_sphere.indexBufferOffset = currentOffset_index;

	currentOffset_vertex += shapeData.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, GeoInfo_sphere.vertexBufferOffset, GeoInfo_sphere.vertexBufferSize, shapeData.verts);
	currentOffset_index += shapeData.indexBufferSize();
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, GeoInfo_sphere.indexBufferOffset, GeoInfo_sphere.indexBufferSize, shapeData.indices);
#pragma endregion
#pragma region TORUS
	shapeData = Neumont::ShapeGenerator::makeTorus(10);
	GeoInfo_torus.numIndices = shapeData.numIndices;
	GeoInfo_torus.vertexBufferSize = shapeData.vertexBufferSize();
	GeoInfo_torus.vertexBufferOffset = currentOffset_vertex;
	GeoInfo_torus.indexBufferSize = shapeData.indexBufferSize(); 
	GeoInfo_torus.indexBufferOffset = currentOffset_index; 

	currentOffset_vertex += shapeData.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, GeoInfo_torus.vertexBufferOffset, GeoInfo_torus.vertexBufferSize, shapeData.verts);
	currentOffset_index += shapeData.indexBufferSize();
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, GeoInfo_torus.indexBufferOffset, GeoInfo_torus.indexBufferSize, shapeData.indices);
#pragma endregion
#pragma region PLANE
	shapeData = Neumont::ShapeGenerator::makePlane(10);
	GeoInfo_plane.numIndices = shapeData.numIndices;
	GeoInfo_plane.vertexBufferSize = shapeData.vertexBufferSize();
	GeoInfo_plane.vertexBufferOffset = currentOffset_vertex;
	GeoInfo_plane.indexBufferSize = shapeData.indexBufferSize(); 
	GeoInfo_plane.indexBufferOffset = currentOffset_index;

	currentOffset_vertex += shapeData.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, GeoInfo_plane.vertexBufferOffset, GeoInfo_plane.vertexBufferSize, shapeData.verts);
	currentOffset_index += shapeData.indexBufferSize();
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, GeoInfo_plane.indexBufferOffset, GeoInfo_plane.indexBufferSize, shapeData.indices);
#pragma endregion
#pragma region TEAPOT
	shapeData = Neumont::ShapeGenerator::makeTeapot(6, glm::mat4());
	GeoInfo_teapot.numIndices = shapeData.numIndices;
	GeoInfo_teapot.vertexBufferSize = shapeData.vertexBufferSize();
	GeoInfo_teapot.vertexBufferOffset = currentOffset_vertex;
	GeoInfo_teapot.indexBufferSize = shapeData.indexBufferSize(); 
	GeoInfo_teapot.indexBufferOffset = currentOffset_index; 

	currentOffset_vertex += shapeData.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, GeoInfo_teapot.vertexBufferOffset, GeoInfo_teapot.vertexBufferSize, shapeData.verts);
	currentOffset_index += shapeData.indexBufferSize();
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, GeoInfo_teapot.indexBufferOffset, GeoInfo_teapot.indexBufferSize, shapeData.indices);
#pragma endregion

#pragma region CHECK DATA
	qDebug();
	qDebug() << endl <<"CUBE:";
	qDebug() << "index-size:  " << GeoInfo_cube.indexBufferSize;
	qDebug() << "vertex-size: " << GeoInfo_cube.vertexBufferSize;
	qDebug() << "index-offset:  " << GeoInfo_cube.indexBufferOffset;
	qDebug() << "vertex-offset: " << GeoInfo_cube.vertexBufferOffset;

	qDebug() << endl <<"ARROW:";
	qDebug() << "index-size:  " << GeoInfo_arrow.indexBufferSize;
	qDebug() << "vertex-size: " << GeoInfo_arrow.vertexBufferSize;
	qDebug() << "index-offset:  " << GeoInfo_arrow.indexBufferOffset;
	qDebug() << "vertex-offset: " << GeoInfo_arrow.vertexBufferOffset;

	qDebug() << endl <<"SPHERE:";
	qDebug() << "index-size:  " << GeoInfo_sphere.indexBufferSize;
	qDebug() << "vertex-size: " << GeoInfo_sphere.vertexBufferSize;
	qDebug() << "index-offset:  " << GeoInfo_sphere.indexBufferOffset;
	qDebug() << "vertex-offset: " << GeoInfo_sphere.vertexBufferOffset;

	qDebug() << endl <<"TORUS:";
	qDebug() << "index-size:  " << GeoInfo_torus.indexBufferSize;
	qDebug() << "vertex-size: " << GeoInfo_torus.vertexBufferSize;
	qDebug() << "index-offset:  " << GeoInfo_torus.indexBufferOffset;
	qDebug() << "vertex-offset: " << GeoInfo_torus.vertexBufferOffset;

	qDebug() << endl <<"PLANE:";
	qDebug() << "index-size:  " << GeoInfo_plane.indexBufferSize;
	qDebug() << "vertex-size: " << GeoInfo_plane.vertexBufferSize;
	qDebug() << "index-offset:  " << GeoInfo_plane.indexBufferOffset;
	qDebug() << "vertex-offset: " << GeoInfo_plane.vertexBufferOffset;

	qDebug() << endl << "TEAPOT:";
	qDebug() << "index-size:  " << GeoInfo_teapot.indexBufferSize;
	qDebug() << "vertex-size: " << GeoInfo_teapot.vertexBufferSize;
	qDebug() << "index-offset:  " << GeoInfo_teapot.indexBufferOffset;
	qDebug() << "vertex-offset: " << GeoInfo_teapot.vertexBufferOffset;
#pragma endregion

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, BUFFER_OFFSET(Neumont::Vertex::COLOR_OFFSET));

}

void render_widget::createShaders()
{
	//programID = ShaderManager::CreateProgram(programID, vertexShaderID, fragShaderID, "vshader.sh", "fshader.sh");
	ShaderManager::CreateProgram(&programID, &vertexShaderID, &fragShaderID, "vshader.sh", "fshader.sh");
}

void render_widget::resizeEvent(QResizeEvent* e)
{
	glViewport(0,0,width(), height());
}

void render_widget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::mat4 cameraView = camera.GetView();
	glm::mat4 projection = glm::perspective(
		60.0f,
		(float)width() / height(),
		0.1f, 100.0f);
	glm::mat4 model;
	glm::mat4 mvp;
	GLuint MVPLocation = glGetUniformLocation(programID, "mvp");

#pragma region CUBE
	model = glm::translate(0.0f,0.0f,10.0f);
	mvp = projection * cameraView * model;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, BUFFER_OFFSET(Neumont::Vertex::POSITION_OFFSET + GeoInfo_cube.vertexBufferOffset));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, BUFFER_OFFSET(Neumont::Vertex::COLOR_OFFSET + GeoInfo_cube.vertexBufferOffset));
	glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &mvp[0][0]);
	glDrawElements(GL_TRIANGLES, GeoInfo_cube.numIndices, GL_UNSIGNED_SHORT, reinterpret_cast<const GLvoid*>(GeoInfo_cube.indexBufferOffset));
#pragma endregion
#pragma region ARROW
	model = glm::translate(0.0f,4.0f,10.0f);
	mvp = projection * cameraView * model;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, BUFFER_OFFSET(Neumont::Vertex::POSITION_OFFSET + GeoInfo_arrow.vertexBufferOffset));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, BUFFER_OFFSET(Neumont::Vertex::COLOR_OFFSET + GeoInfo_arrow.vertexBufferOffset));
	glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &mvp[0][0]);
	glDrawElements(GL_TRIANGLES, GeoInfo_arrow.numIndices, GL_UNSIGNED_SHORT, reinterpret_cast<const GLvoid*>(GeoInfo_arrow.indexBufferOffset));
#pragma endregion
#pragma region SPHERE
	model = glm::translate(5.0f,0.0f,10.0f);
	mvp = projection * cameraView * model;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, BUFFER_OFFSET(Neumont::Vertex::POSITION_OFFSET + GeoInfo_sphere.vertexBufferOffset));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, BUFFER_OFFSET(Neumont::Vertex::COLOR_OFFSET + GeoInfo_sphere.vertexBufferOffset));
	glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &mvp[0][0]);
	glDrawElements(GL_TRIANGLES, GeoInfo_sphere.numIndices, GL_UNSIGNED_SHORT, reinterpret_cast<const GLvoid*>(GeoInfo_sphere.indexBufferOffset));
#pragma endregion
#pragma region TORUS
	model = glm::translate(-5.0f,0.0f,10.0f);
	mvp = projection * cameraView * model;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, BUFFER_OFFSET(Neumont::Vertex::POSITION_OFFSET + GeoInfo_torus.vertexBufferOffset));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, BUFFER_OFFSET(Neumont::Vertex::COLOR_OFFSET + GeoInfo_torus.vertexBufferOffset));
	glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &mvp[0][0]);
	glDrawElements(GL_TRIANGLES, GeoInfo_torus.numIndices, GL_UNSIGNED_SHORT, reinterpret_cast<const GLvoid*>(GeoInfo_torus.indexBufferOffset));
#pragma endregion
#pragma region PLANE
	model = glm::translate(0.0f,-5.0f,10.0f);
	mvp = projection * cameraView * model;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, BUFFER_OFFSET(Neumont::Vertex::POSITION_OFFSET + GeoInfo_plane.vertexBufferOffset));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, BUFFER_OFFSET(Neumont::Vertex::COLOR_OFFSET + GeoInfo_plane.vertexBufferOffset));
	glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &mvp[0][0]);
	glDrawElements(GL_TRIANGLES, GeoInfo_plane.numIndices, GL_UNSIGNED_SHORT, reinterpret_cast<const GLvoid*>(GeoInfo_plane.indexBufferOffset));
#pragma endregion
#pragma region TEAPOT
	model = glm::translate(0.0f,0.0f,15.0f) * 
		glm::rotate(glm::mat4(), -90.0f, 1.0f, 0.0f, 0.0f);
	mvp = projection * cameraView * model;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, BUFFER_OFFSET(Neumont::Vertex::POSITION_OFFSET + GeoInfo_teapot.vertexBufferOffset));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, BUFFER_OFFSET(Neumont::Vertex::COLOR_OFFSET + GeoInfo_teapot.vertexBufferOffset));
	glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &mvp[0][0]);
	glDrawElements(GL_TRIANGLES, GeoInfo_teapot.numIndices, GL_UNSIGNED_SHORT, reinterpret_cast<const GLvoid*>(GeoInfo_teapot.indexBufferOffset));
#pragma endregion
}