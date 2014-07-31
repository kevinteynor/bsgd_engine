#ifndef A_STAR_PATH_H
#define A_STAR_PATH_H

#include <glm\glm.hpp>
#include <vector>
#include "EngineTypeDefs.h"
#include "ExportHeader.h"
#include "FastDelegate.h"

struct AStarNode_Dynamic;
struct AStarNode_Static;

#pragma warning (disable : 4251)
struct ENGINE_SHARED AStarConnection
{
	ushort connectorID;
	float weight;

	AStarConnection() : connectorID(0), weight(0){}
	AStarConnection(ushort nID, float w);
	AStarConnection(AStarNode_Dynamic* n, float w);
};

struct ENGINE_SHARED AStarNode_Dynamic
{
	ushort ID;
	glm::vec3 position;
	uint numConnections;
	std::vector<AStarConnection> connections;

	AStarNode_Dynamic(ushort id, const glm::vec3& pos = glm::vec3(0,0,0));
	AStarNode_Dynamic(const AStarNode_Static& node);

	void AddConnection(ushort cID, float weight);
	void AddConnection(const AStarNode_Dynamic& node);
	void RemoveConnection(ushort cID);
	void RemoveConnection(const AStarNode_Dynamic& node);
};
struct ENGINE_SHARED AStarNode_Static
{
	ushort ID;
	glm::vec3 position;
	uint numConnections;
	AStarConnection* connections;

	AStarNode_Static() {}
	AStarNode_Static(ushort id, uint numconnections, const glm::vec3& pos = glm::vec3(0,0,0));
	AStarNode_Static(const AStarNode_Static& node);
	AStarNode_Static(const AStarNode_Dynamic& node);
	~AStarNode_Static() {}
	AStarNode_Static& operator=(const AStarNode_Static& other);

	void SetConnection(uint cIndex, ushort cID, float w);
	void CleanUp(){ delete[] connections; }
};

struct ENGINE_SHARED AStarPath_NodeRecord
{
	AStarNode_Static* node;
	AStarNode_Static* connection;
	float costSoFar;
	float estimatedTotalCost;

	~AStarPath_NodeRecord() { node = 0; connection = 0; }
};
struct ENGINE_SHARED AStarPath
{
public:
	uint currentIndex;
	uint indexCount;
	uint* nodeIndices;

	AStarPath(uint* nodes = 0, uint count = 0, uint startIndex = 0) : currentIndex(startIndex), indexCount(count), nodeIndices(nodes) {}

	static AStarPath FindPath(AStarNode_Static* nodes, uint startIndex, uint endIndex);
	static AStarPath FindPath(AStarNode_Static* nodes, uint numNodes, glm::vec3 start, glm::vec3 end);

	static AStarPath FindPath(AStarNode_Static* nodes, uint startIndex, uint endIndex, fastdelegate::FastDelegate2<uint, uint, float> heuristic);
	static AStarPath FindPath(AStarNode_Static* nodes, uint numNodes, glm::vec3 start, glm::vec3 end, fastdelegate::FastDelegate2<uint, uint, float> heuristic);
};

#endif A_STAR_PATH_H