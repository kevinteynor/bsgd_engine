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
	GLuint plane_vertexBufferID;
	GLuint plane_indexBufferID;
	GLuint plane_numIndices;

	GLuint torus_vertexBufferID;
	GLuint torus_indexBufferID;
	GLuint torus_numIndices;

	GLuint teapot_vertexBufferID;
	GLuint teapot_indexBufferID;
	GLuint teapot_numIndices;

	GLuint cube_vertexBufferID;
	GLuint cube_indexBufferID;
	GLuint cube_numIndices;

	GLuint textureID;
	GLuint slimeTexID;
	GLuint smileTexID;

#pragma region Shader programs
	GLuint vertexShaderID;
	GLuint fragShaderID;
	GLuint programID;

	GLuint vertexShaderID_2;
	GLuint fragShaderID_2;
	GLuint fragProgramID;

	GLuint simpleVertexShaderID;
	GLuint simpleProgramID;

	GLuint vertexShader_texture_ID;
	GLuint fragShader_texture_ID;
	GLuint program_texture_ID;

	GLuint vertexShader_texture_2_ID;
	GLuint fragShader_texture_2_ID;
	GLuint program_texture_2_ID;

	GLuint vertexShader_discard_ID;
	GLuint fragShader_discard_ID;
	GLuint program_discard_ID;
#pragma endregion

	glm::vec2 lastMousePos;
	
	DebugCamera camera;

	void compileShader(const char* fileName, GLuint shaderID);
	void loadTextures();

protected:
	void initializeGL();
	void shutdown();
	
	void keyPressEvent(QKeyEvent *e);
	void wheelEvent(QWheelEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);

	void fillBuffer();
	void createShaders();

	void resizeEvent(QResizeEvent *e);

	void paintGL();
	void paintGL_DISCARD();

public:
	bool RunFragShader;
	bool discardShader;

	glm::vec3 lightPosition;
	glm::vec3 lightColor;
	glm::vec3 ambientColor;

	glm::vec3 planeAmbient;
	glm::vec3 planeDiffuse;
	glm::vec3 planeSpecular;

	glm::vec3 torusAmbient;
	glm::vec3 torusDiffuse;
	glm::vec3 torusSpecular;

	glm::vec3 teapotAmbient;
	glm::vec3 teapotDiffuse;
	glm::vec3 teapotSpecular;

	MyGLWidget();

	void updateShaders();

};

#endif // MY_GL_WINDOW