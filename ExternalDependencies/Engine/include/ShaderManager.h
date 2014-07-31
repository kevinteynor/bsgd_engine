#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include "ExportHeader.h"
#include "TypeDefs.h"

namespace ShaderManager
{
	void ENGINE_SHARED CompileShader(const char* fileName, uint shaderID);
	void ENGINE_SHARED CreateProgram(uint* programID, uint* vertexShaderID, uint* fragShaderID, const char* vertexFile, const char* fragFile);	
	uint ENGINE_SHARED CreateProgram(const char* vertexFile, const char* fragFile);
	uint ENGINE_SHARED CreateProgram_CHARARRAY(const char* vertexString, const char* fragString);
	void ENGINE_SHARED CreateSimpleShaderProgram(uint* programID, uint* vertexShaderID, uint* fragShaderID);
}

#endif // SHADER_MANAGER_H