#include <GL\glew.h>
#include <Qt\qdebug.h>
#include <Qt\qfile.h>
#include <Qt\qtextstream.h>
#include "passthroughShaders.inl"
#include "ShaderManager.h"

using namespace ShaderManager;

void ShaderManager::CompileShader(const char* fileName, uint shaderID)
{
	QFile input(fileName);
	if(! input.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "File failed to open: " << fileName;
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

		qDebug() << buffer;

		delete[] buffer;
	}
}
void ShaderManager::CreateProgram(uint* programID, uint* vertexShaderID, uint* fragShaderID, const char* vertexFile, const char* fragFile)
{
	*vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	*fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	*programID = glCreateProgram();

	CompileShader(vertexFile, *vertexShaderID);
	CompileShader(fragFile, *fragShaderID);

	glAttachShader(*programID, *vertexShaderID);
	glAttachShader(*programID, *fragShaderID);
	glLinkProgram(*programID);
}
uint ShaderManager::CreateProgram(const char* vertexFile, const char* fragFile)
{
	uint vShader = glCreateShader(GL_VERTEX_SHADER);
	uint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	uint programID = glCreateProgram();

	CompileShader(vertexFile, vShader);
	CompileShader(fragFile, fShader);

	glAttachShader(programID, vShader);
	glAttachShader(programID, fShader);
	glLinkProgram(programID);

	return programID;
}
uint ShaderManager::CreateProgram_CHARARRAY(const char* vertexString, const char* fragString)
{
	uint vShader = glCreateShader(GL_VERTEX_SHADER);
	uint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	uint programID = glCreateProgram();

	qDebug() << vertexString;
	const char* buf[1];
	buf[0] = vertexString;
	glShaderSource(vShader, 1, buf, NULL);
	glCompileShader(vShader);

	GLint status;
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &status);
	if(status != GL_TRUE)
	{
		GLint infoLogLen;
		glGetShaderiv(vShader, GL_INFO_LOG_LENGTH, &infoLogLen);

		char* buffer = new char[infoLogLen];
		GLsizei bitBucket;
		glGetShaderInfoLog(vShader, infoLogLen, &bitBucket, buffer);

		qDebug() << buffer;

		delete[] buffer;
	}


	qDebug() << fragString;
	buf[0] = fragString;
	glShaderSource(fShader, 1, buf, NULL);
	glCompileShader(fShader);

	glGetShaderiv(fShader, GL_COMPILE_STATUS, &status);
	if(status != GL_TRUE)
	{
		GLint infoLogLen;
		glGetShaderiv(fShader, GL_INFO_LOG_LENGTH, &infoLogLen);

		char* buffer = new char[infoLogLen];
		GLsizei bitBucket;
		glGetShaderInfoLog(fShader, infoLogLen, &bitBucket, buffer);

		qDebug() << buffer;

		delete[] buffer;
	}



	glAttachShader(programID, vShader);
	glAttachShader(programID, fShader);
	glLinkProgram(programID);

	qDebug() << "programID:" <<  programID;

	return programID;
}
void ShaderManager::CreateSimpleShaderProgram(uint* programID, uint* vertexShaderID, uint* fragShaderID)
{
	*vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	*fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	*programID = glCreateProgram();
	
	glShaderSource(*vertexShaderID, 1, &passthrough_vertex_code, NULL);
	glCompileShader(*vertexShaderID);

	glShaderSource(*fragShaderID, 1, &passthrough_fragment_code, NULL);
	glCompileShader(*fragShaderID);

	glAttachShader(*programID, *vertexShaderID);
	glAttachShader(*programID, *fragShaderID);
	glLinkProgram(*programID);
}