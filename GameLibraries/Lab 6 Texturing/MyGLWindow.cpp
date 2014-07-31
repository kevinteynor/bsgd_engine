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
#include "ShaderManager.h"

#define JAMIE_CONTROL
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
#define TURN_U Qt::Key_T
#define TURN_D Qt::Key_G
#define UP Qt::Key_R
#define DOWN Qt::Key_F

#define MOUSE_TRACKING false

#endif // JAMIE_CONTROLS

MyGLWidget::MyGLWidget()
{
#pragma region Initialize Scene
	lightPosition = glm::vec3(0,2,0);
	lightColor = glm::vec3(1,1,1);
	ambientColor = glm::vec3(0.1f,0.1f,0.1f);

	planeAmbient = glm::vec3(0.5f, 0.5f, 0.5f);
	planeDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	planeSpecular = glm::vec3(0.5f, 0.5f, 0.5f);

	torusAmbient = glm::vec3(0.5f, 0.5f, 0.5f);
	torusDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	torusSpecular = glm::vec3(0.5f, 0.5f, 0.5f);

	teapotAmbient = glm::vec3(0.5f, 0.5f, 0.5f);
	teapotDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	teapotSpecular = glm::vec3(0.5f, 0.5f, 0.5f);
#pragma endregion
}

void MyGLWidget::initializeGL()
{
	glewInit();

	setMinimumHeight(900);

	setFocusPolicy(Qt::StrongFocus);

	setMouseTracking(MOUSE_TRACKING);

	camera.SetPosition(-10,6,0);
	camera.SetTarget(10,-6,0);
	lastMousePos = glm::vec2(-1,-1);
	RunFragShader = false;
	discardShader = false;

	glEnable(GL_DEPTH_TEST);

	glClearColor(0,0,0,1);
	fillBuffer();
	createShaders();

	loadTextures();

	qDebug() << "initializeGL() finished";
}
void MyGLWidget::shutdown()
{
	glDeleteBuffers(1, &cube_indexBufferID);
	glDeleteBuffers(1, &plane_indexBufferID);
	glDeleteBuffers(1, &torus_indexBufferID);
	glDeleteBuffers(1, &teapot_indexBufferID);

	glDeleteBuffers(1, &cube_vertexBufferID);
	glDeleteBuffers(1, &plane_vertexBufferID);
	glDeleteBuffers(1, &torus_vertexBufferID);
	glDeleteBuffers(1, &teapot_vertexBufferID);
}

void MyGLWidget::keyPressEvent(QKeyEvent *e)
{
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

	glDraw();
}
void MyGLWidget::wheelEvent(QWheelEvent *e)
{
	camera.Move_Forward((float)(e->delta() * 0.005f));

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

bool bufferFilled = false;
void MyGLWidget::fillBuffer()
{
	if(bufferFilled)
		return;

#pragma region cube
	Neumont::ShapeData cube_shapeData = Neumont::ShapeGenerator::makeCube();
	cube_numIndices = cube_shapeData.numIndices;

	glGenBuffers(1, &cube_vertexBufferID);
	glGenBuffers(1, &cube_indexBufferID);

	glBindBuffer(GL_ARRAY_BUFFER, cube_vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, cube_shapeData.vertexBufferSize(), cube_shapeData.verts, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube_shapeData.indexBufferSize(), cube_shapeData.indices, GL_STATIC_DRAW);
	cube_shapeData.cleanUp();
#pragma endregion
#pragma region plane
	Neumont::ShapeData plane_shapeData = Neumont::ShapeGenerator::makePlane(10);
	plane_numIndices = plane_shapeData.numIndices;

	glGenBuffers(1, &plane_vertexBufferID);
	glGenBuffers(1, &plane_indexBufferID);
	
	glBindBuffer(GL_ARRAY_BUFFER, plane_vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, plane_shapeData.vertexBufferSize(), plane_shapeData.verts, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, plane_indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, plane_shapeData.indexBufferSize(), plane_shapeData.indices, GL_STATIC_DRAW);
	plane_shapeData.cleanUp();
#pragma endregion
#pragma region cube_2
	Neumont::ShapeData torus_shapeData = Neumont::ShapeGenerator::makeCube();
	torus_numIndices = torus_shapeData.numIndices;

	glGenBuffers(1, &torus_vertexBufferID);
	glGenBuffers(1, &torus_indexBufferID);

	glBindBuffer(GL_ARRAY_BUFFER, torus_vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, torus_shapeData.vertexBufferSize(), torus_shapeData.verts, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, torus_indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, torus_shapeData.indexBufferSize(), torus_shapeData.indices, GL_STATIC_DRAW);
	torus_shapeData.cleanUp();
#pragma endregion
#pragma region teapot
	Neumont::ShapeData teapot_shapeData = Neumont::ShapeGenerator::makeTeapot(6, glm::mat4());
	teapot_numIndices = teapot_shapeData.numIndices;
	
	glGenBuffers(1, &teapot_vertexBufferID);
	glGenBuffers(1, &teapot_indexBufferID);
	
	glBindBuffer(GL_ARRAY_BUFFER, teapot_vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, teapot_shapeData.vertexBufferSize(), teapot_shapeData.verts, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, teapot_indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, teapot_shapeData.indexBufferSize(), teapot_shapeData.indices, GL_STATIC_DRAW);
	teapot_shapeData.cleanUp();
#pragma endregion
// Tell GL how to read the bits
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, BUFFER_OFFSET(Neumont::Vertex::COLOR_OFFSET));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, BUFFER_OFFSET(Neumont::Vertex::NORMAL_OFFSET));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, BUFFER_OFFSET(Neumont::Vertex::UV_OFFSET));
}
void MyGLWidget::updateShaders()
{
	if(!bufferFilled)
		fillBuffer();

	this->updateGL();
	this->repaint();
}

void MyGLWidget::loadTextures()
{
	glEnable(GL_TEXTURE_2D);

	glGenTextures(1, &textureID);
	glGenTextures(1, &slimeTexID);
	glGenTextures(1, &smileTexID);	

	// load texture
	QImage textureImage = QGLWidget::convertToGLFormat(QImage("texture.png", "PNG"));
	glBindTexture(GL_TEXTURE_2D, textureID);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureImage.width(),
		textureImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
		textureImage.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	textureImage = QGLWidget::convertToGLFormat(QImage("slime.png", "PNG"));
	glBindTexture(GL_TEXTURE_2D, slimeTexID);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureImage.width(),
		textureImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
		textureImage.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	textureImage = QGLWidget::convertToGLFormat(QImage("smile.png", "PNG"));
	glBindTexture(GL_TEXTURE_2D, smileTexID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureImage.width(),
		textureImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
		textureImage.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, slimeTexID);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, smileTexID);
}

void MyGLWidget::createShaders()
{
	if(!bufferFilled)
		fillBuffer();

	programID = ShaderManager::CreateProgram(programID, vertexShaderID, fragShaderID, "MyVertexShader.sh", "MyFragmentShader.sh");
	simpleProgramID = ShaderManager::CreateProgram(simpleProgramID, simpleVertexShaderID, fragShaderID, "SimpleVertexShader.sh", "MyFragmentShader.sh");
	ShaderManager::CreateSimpleShaderProgram(&simpleProgramID, &simpleVertexShaderID, &fragShaderID);
	fragProgramID = ShaderManager::CreateProgram(fragProgramID, vertexShaderID_2, fragShaderID_2, "VertexShader_2.sh", "FragShader_2.sh");
	program_texture_ID = ShaderManager::CreateProgram(program_texture_ID, vertexShader_texture_ID, fragShader_texture_ID, "vertexshader_texture.vsh", "fragshader_texture.fsh");
	program_texture_2_ID = ShaderManager::CreateProgram(program_texture_2_ID, vertexShaderID_2, fragShaderID_2, "vertexshader_texture_2.vsh", "fragshader_texture_2.fsh");
	program_discard_ID = ShaderManager::CreateProgram(program_discard_ID, vertexShader_discard_ID, fragShader_discard_ID, "vertex_discard.vsh", "fragment_discard.fsh");

	glUseProgram(programID);

	/*vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	simpleVertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	vertexShaderID_2 = glCreateShader(GL_VERTEX_SHADER);
	fragShaderID_2 = glCreateShader(GL_FRAGMENT_SHADER);

	programID = glCreateProgram();
	simpleProgramID = glCreateProgram();
	fragProgramID = glCreateProgram();
	

	compileShader("MyVertexShader.sh", vertexShaderID);
	compileShader("MyFragmentShader.sh", fragShaderID);
	compileShader("SimpleVertexShader.sh", simpleVertexShaderID);
	compileShader("VertexShader_2.sh", vertexShaderID_2);
	compileShader("FragShader_2.sh", fragShaderID_2);
	
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragShaderID);
	glLinkProgram(programID);

	glAttachShader(simpleProgramID, simpleVertexShaderID);
	glAttachShader(simpleProgramID, fragShaderID);
	glLinkProgram(simpleProgramID);

	glAttachShader(fragProgramID, vertexShaderID_2);
	glAttachShader(fragProgramID, fragShaderID_2);
	glLinkProgram(fragProgramID);

	glUseProgram(programID);*/
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
		exit(1);
	}
}

void MyGLWidget::resizeEvent(QResizeEvent *e)
{
	glViewport(0,0, width(), height());
}

void MyGLWidget::paintGL()
{
	if(discardShader)
	{
		paintGL_DISCARD();
		return;
	}

	GLuint currentProgramID = program_texture_ID;
	if(RunFragShader)
		currentProgramID = program_texture_2_ID;

	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::mat4 cameraView = camera.GetView();
	glm::mat4 projection = glm::perspective(
		60.0f, 
		((float)width()) / height(),
		0.1f, 100.0f);

	// TEXTURE
	GLuint texloc = glGetUniformLocation(currentProgramID, "texture1");
	glUniform1i(texloc, 0);

	GLuint slimeloc = glGetUniformLocation(currentProgramID, "slime1");
	glUniform1i(slimeloc, 1);

	GLuint ambientLocation = glGetUniformLocation(currentProgramID, "ambient_global");	
	GLuint lightPositionLocation = glGetUniformLocation(currentProgramID, "light_position");
	GLuint lightColorLocation = glGetUniformLocation(currentProgramID, "light_color");
	GLuint eyePositionLocation = glGetUniformLocation(currentProgramID, "eye_pos");
	

	glUniform3f(ambientLocation, ambientColor.x, ambientColor.y, ambientColor.z);
	glUniform3f(lightPositionLocation, lightPosition.x, lightPosition.y, lightPosition.z);	
	glUniform3f(lightColorLocation, lightColor.x, lightColor.y, lightColor.z);
	glUniform3f(eyePositionLocation, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().y);

	GLuint mvpLocation = glGetUniformLocation(currentProgramID, "mvp");
	GLuint matAmbientLocation = glGetUniformLocation(currentProgramID, "ambient_material");
	GLuint matDiffuseLocation = glGetUniformLocation(currentProgramID, "diffuse_material");
	GLuint matSpecularLocation = glGetUniformLocation(currentProgramID, "specular_material");
	GLuint eyePosLocation = glGetUniformLocation(currentProgramID, "eye_pos");
	GLuint modelToWorldNormalMatLocation = glGetUniformLocation(currentProgramID, "modelToWorld_Normal");
	GLuint modelToWorldMatLocation = glGetUniformLocation(currentProgramID, "modelToWorld");
	glm::vec3 camPos = camera.GetPosition();

	glUseProgram(simpleProgramID);
#pragma region cube

	glBindBuffer(GL_ARRAY_BUFFER, cube_vertexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_indexBufferID);

	GLuint simple_mvpLocation = glGetUniformLocation(simpleProgramID, "mvp");

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, BUFFER_OFFSET(Neumont::Vertex::COLOR_OFFSET));

	glm::mat4 model = glm::translate(lightPosition.x, lightPosition.y, lightPosition.z) * 
		glm::scale(0.1f, 0.1f, 0.1f);
	glm::mat4 fullTransform = projection * cameraView * model;

	glUniformMatrix4fv(simple_mvpLocation, 1, GL_FALSE, &fullTransform[0][0]);

	glDrawElements(GL_TRIANGLES, cube_numIndices, GL_UNSIGNED_SHORT, NULL);
#pragma endregion

	glUseProgram(currentProgramID);
#pragma region plane
	glBindBuffer(GL_ARRAY_BUFFER, plane_vertexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, plane_indexBufferID);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, 0);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, BUFFER_OFFSET(Neumont::Vertex::NORMAL_OFFSET));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, BUFFER_OFFSET(Neumont::Vertex::UV_OFFSET));
	
	model = glm::translate(0,0,0);//glm::scale(2.0f,2.0f,2.0f);
	fullTransform = projection * cameraView * model;	
	
	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &fullTransform[0][0]);
	glUniformMatrix3fv(modelToWorldNormalMatLocation, 1, GL_FALSE, &glm::mat3(model)[0][0]);
	glUniformMatrix4fv(modelToWorldMatLocation, 1, GL_FALSE, &model[0][0]);
	glUniform3f(matAmbientLocation, planeAmbient.x, planeAmbient.y, planeAmbient.z);
	glUniform3f(matDiffuseLocation, planeDiffuse.x, planeDiffuse.y, planeDiffuse.z);
	glUniform3f(matSpecularLocation, planeSpecular.x, planeSpecular.y, planeSpecular.z);
	glUniform3f(eyePosLocation, camPos.x, camPos.y, camPos.z);

	glDrawElements(GL_TRIANGLES, plane_numIndices, GL_UNSIGNED_SHORT, NULL);
#pragma endregion
#pragma region cube_2
	glBindBuffer(GL_ARRAY_BUFFER, torus_vertexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, torus_indexBufferID);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, 0);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, BUFFER_OFFSET(Neumont::Vertex::NORMAL_OFFSET));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, BUFFER_OFFSET(Neumont::Vertex::UV_OFFSET));

	model = glm::translate(-1.0f, 0.5f, 2.0f);
	fullTransform = projection * cameraView * model;

	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &fullTransform[0][0]);
	glUniformMatrix3fv(modelToWorldNormalMatLocation, 1, GL_FALSE, &glm::mat3(model)[0][0]);
	glUniformMatrix4fv(modelToWorldMatLocation, 1, GL_FALSE, &model[0][0]);
	glUniform3f(matAmbientLocation, torusAmbient.x, torusAmbient.y, torusAmbient.z);
	glUniform3f(matDiffuseLocation, torusDiffuse.x, torusDiffuse.y, torusDiffuse.z);
	glUniform3f(matSpecularLocation, torusSpecular.x, torusSpecular.y, torusSpecular.z);
	glUniform3f(eyePosLocation, camPos.x, camPos.y, camPos.z);

	glDrawElements(GL_TRIANGLES, torus_numIndices, GL_UNSIGNED_SHORT, NULL);
#pragma endregion
#pragma region teapot
	glBindBuffer(GL_ARRAY_BUFFER, teapot_vertexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, teapot_indexBufferID);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, 0);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, BUFFER_OFFSET(Neumont::Vertex::NORMAL_OFFSET));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, BUFFER_OFFSET(Neumont::Vertex::UV_OFFSET));
	
	model = 
		glm::translate(2.0f, 0.0f, -3.0f) *
		glm::rotate(glm::mat4(), -60.0f, 0.0f, 1.0f, 0.0f) * 
		glm::rotate(glm::mat4(), -90.0f, 1.0f, 0.0f, 0.0f);
	fullTransform = projection * cameraView * model;

	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &fullTransform[0][0]);
	glUniformMatrix3fv(modelToWorldNormalMatLocation, 1, GL_FALSE, &glm::mat3(model)[0][0]);
	glUniformMatrix4fv(modelToWorldMatLocation, 1, GL_FALSE, &model[0][0]);
	glUniform3fv(matAmbientLocation, 1, &teapotAmbient[0]);
	glUniform3fv(matDiffuseLocation, 1, &teapotDiffuse[0]);
	glUniform3fv(matSpecularLocation, 1, &teapotSpecular[0]);
	glUniform3fv(eyePosLocation, 1, &camera.GetPosition()[0]);

	glDrawElements(GL_TRIANGLES, teapot_numIndices, GL_UNSIGNED_SHORT, NULL);
#pragma endregion
}
void MyGLWidget::paintGL_DISCARD()
{
	GLuint currentProgramID = program_discard_ID;

	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::mat4 cameraView = camera.GetView();
	glm::mat4 projection = glm::perspective(
		60.0f, 
		((float)width()) / height(),
		0.1f, 100.0f);

	// TEXTURE
	GLuint slimeloc = glGetUniformLocation(currentProgramID, "discardTex");
	glUniform1i(slimeloc, 2);

	GLuint ambientLocation = glGetUniformLocation(currentProgramID, "ambient_global");	
	GLuint lightPositionLocation = glGetUniformLocation(currentProgramID, "light_position");	
	GLuint lightColorLocation = glGetUniformLocation(currentProgramID, "light_color");	
	GLuint eyePositionLocation = glGetUniformLocation(currentProgramID, "eye_pos");
	

	glUniform3f(ambientLocation, ambientColor.x, ambientColor.y, ambientColor.z);
	glUniform3f(lightPositionLocation, lightPosition.x, lightPosition.y, lightPosition.z);	
	glUniform3f(lightColorLocation, lightColor.x, lightColor.y, lightColor.z);
	glUniform3f(eyePositionLocation, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().y);

	GLuint mvpLocation = glGetUniformLocation(currentProgramID, "mvp");
	GLuint matAmbientLocation = glGetUniformLocation(currentProgramID, "ambient_material");
	GLuint matDiffuseLocation = glGetUniformLocation(currentProgramID, "diffuse_material");
	GLuint matSpecularLocation = glGetUniformLocation(currentProgramID, "specular_material");
	GLuint eyePosLocation = glGetUniformLocation(currentProgramID, "eye_pos");
	GLuint modelToWorldNormalMatLocation = glGetUniformLocation(currentProgramID, "modelToWorld_Normal");
	GLuint modelToWorldMatLocation = glGetUniformLocation(currentProgramID, "modelToWorld");
	glm::vec3 camPos = camera.GetPosition();

	glUseProgram(simpleProgramID);
#pragma region cube

	glBindBuffer(GL_ARRAY_BUFFER, cube_vertexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_indexBufferID);

	GLuint simple_mvpLocation = glGetUniformLocation(simpleProgramID, "mvp");

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, BUFFER_OFFSET(Neumont::Vertex::COLOR_OFFSET));

	glm::mat4 model = glm::translate(lightPosition.x, lightPosition.y, lightPosition.z) * 
		glm::scale(0.1f, 0.1f, 0.1f);
	glm::mat4 fullTransform = projection * cameraView * model;

	glUniformMatrix4fv(simple_mvpLocation, 1, GL_FALSE, &fullTransform[0][0]);

	glDrawElements(GL_TRIANGLES, cube_numIndices, GL_UNSIGNED_SHORT, NULL);
#pragma endregion

	glUseProgram(currentProgramID);
#pragma region plane
	glBindBuffer(GL_ARRAY_BUFFER, plane_vertexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, plane_indexBufferID);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, 0);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, BUFFER_OFFSET(Neumont::Vertex::NORMAL_OFFSET));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, BUFFER_OFFSET(Neumont::Vertex::UV_OFFSET));
	
	model = glm::translate(0,0,0);//glm::scale(2.0f,2.0f,2.0f);
	fullTransform = projection * cameraView * model;	
	
	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &fullTransform[0][0]);
	glUniformMatrix3fv(modelToWorldNormalMatLocation, 1, GL_FALSE, &glm::mat3(model)[0][0]);
	glUniformMatrix4fv(modelToWorldMatLocation, 1, GL_FALSE, &model[0][0]);
	glUniform3f(matAmbientLocation, planeAmbient.x, planeAmbient.y, planeAmbient.z);
	glUniform3f(matDiffuseLocation, planeDiffuse.x, planeDiffuse.y, planeDiffuse.z);
	glUniform3f(matSpecularLocation, planeSpecular.x, planeSpecular.y, planeSpecular.z);
	glUniform3f(eyePosLocation, camPos.x, camPos.y, camPos.z);

	glDrawElements(GL_TRIANGLES, plane_numIndices, GL_UNSIGNED_SHORT, NULL);
#pragma endregion
#pragma region cube_2
	glBindBuffer(GL_ARRAY_BUFFER, torus_vertexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, torus_indexBufferID);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, 0);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, BUFFER_OFFSET(Neumont::Vertex::NORMAL_OFFSET));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, BUFFER_OFFSET(Neumont::Vertex::UV_OFFSET));

	model = glm::translate(-1.0f, 0.5f, 2.0f);
	fullTransform = projection * cameraView * model;

	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &fullTransform[0][0]);
	glUniformMatrix3fv(modelToWorldNormalMatLocation, 1, GL_FALSE, &glm::mat3(model)[0][0]);
	glUniformMatrix4fv(modelToWorldMatLocation, 1, GL_FALSE, &model[0][0]);
	glUniform3f(matAmbientLocation, torusAmbient.x, torusAmbient.y, torusAmbient.z);
	glUniform3f(matDiffuseLocation, torusDiffuse.x, torusDiffuse.y, torusDiffuse.z);
	glUniform3f(matSpecularLocation, torusSpecular.x, torusSpecular.y, torusSpecular.z);
	glUniform3f(eyePosLocation, camPos.x, camPos.y, camPos.z);

	glDrawElements(GL_TRIANGLES, torus_numIndices, GL_UNSIGNED_SHORT, NULL);
#pragma endregion
#pragma region teapot
	glBindBuffer(GL_ARRAY_BUFFER, teapot_vertexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, teapot_indexBufferID);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, 0);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, BUFFER_OFFSET(Neumont::Vertex::NORMAL_OFFSET));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, BUFFER_OFFSET(Neumont::Vertex::UV_OFFSET));
	
	model = 
		glm::translate(2.0f, 0.0f, -3.0f) *
		glm::rotate(glm::mat4(), -60.0f, 0.0f, 1.0f, 0.0f) * 
		glm::rotate(glm::mat4(), -90.0f, 1.0f, 0.0f, 0.0f);
	fullTransform = projection * cameraView * model;

	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &fullTransform[0][0]);
	glUniformMatrix3fv(modelToWorldNormalMatLocation, 1, GL_FALSE, &glm::mat3(model)[0][0]);
	glUniformMatrix4fv(modelToWorldMatLocation, 1, GL_FALSE, &model[0][0]);
	glUniform3fv(matAmbientLocation, 1, &teapotAmbient[0]);
	glUniform3fv(matDiffuseLocation, 1, &teapotDiffuse[0]);
	glUniform3fv(matSpecularLocation, 1, &teapotSpecular[0]);
	glUniform3fv(eyePosLocation, 1, &camera.GetPosition()[0]);

	glDrawElements(GL_TRIANGLES, teapot_numIndices, GL_UNSIGNED_SHORT, NULL);
#pragma endregion
}