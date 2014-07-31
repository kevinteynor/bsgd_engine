#ifndef NATIVE_LEVEL_H
#define NATIVE_LEVEL_H

#include <fstream>
#include <GL\glew.h>
#include <vector>
#include "AStarPath.h"
#include "EngineTypeDefs.h"
#include "ExportHeader.h"
#include "Renderer.h"

class ENGINE_SHARED NativeLevel_Dynamic
{
	uint nextNodeID;

	static NativeLevel_Dynamic ReadFile(const char* filepath, Renderer* renderer, uint shader);
	static void ReadHeader(std::ifstream* stream, uint* SEGMENT_STARTS, uint& HEADER_LENGTH);
	static void ReadRenderablesData(char* data, NativeLevel_Dynamic& level, Renderer* renderer, uint shader);
	static void ReadPathingNodesData(char* data, NativeLevel_Dynamic& level);

	void WriteFile(const char* filepath);
	void WriteHeaderInfo(std::ofstream* stream, uint& HEADER_LENGTH, uint* SEGMENT_STARTS);
	void WriteRenderablesData(std::ofstream* stream, const uint& SEGMENT_START);
	void WritePathingNodesData(std::ofstream* stream, const uint& SEGMENT_START);

public:
	std::vector<Renderer::RenderableInstanceInfo*> renderables;
	std::vector<AStarNode_Dynamic> nodes;

	NativeLevel_Dynamic() : nextNodeID(0) {}
	~NativeLevel_Dynamic(){}

	void AddPathingNode(const glm::vec3& p);
	void RemovePathingNode(const uint& ID);

	static NativeLevel_Dynamic Load(const char* filepath, Renderer* renderer, uint shader);
	void Save(const char* filepath);
};

class ENGINE_SHARED NativeLevel_Static
{
private:
	static NativeLevel_Static ReadFile(const char* filepath, Renderer* renderer, uint shader);
	static void ReadHeader(std::ifstream* stream, uint* SEGMENT_STARTS, uint& HEADER_LENGTH);
	static void ReadRenderablesData(char* data, NativeLevel_Static& level, Renderer* renderer, uint shader);
	static void ReadPathingNodesData(char* data, NativeLevel_Static& level);

public:
	Renderer::RenderableInstanceInfo** renderables;
	AStarNode_Static* nodes;

	uint numRenderables;
	uint numNodes;

	NativeLevel_Static() : nodes(0), renderables(0) {}
	~NativeLevel_Static(){}

	static NativeLevel_Static Load(const char* filepath, Renderer* renderer, uint shader);
};

#endif // NATIVE_LEVEL_H