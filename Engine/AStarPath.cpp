#include "AStarPath.h"

#include <algorithm>
#include <float.h>
#include <glm\glm.hpp>
#include <iostream>
#include <vector>
#include "Assertion.h"
#include "EngineTypeDefs.h"

AStarConnection::AStarConnection(ushort nID, float w)
	: connectorID(nID), weight(w) {}
AStarConnection::AStarConnection(AStarNode_Dynamic* n, float w)
{
	connectorID = n->ID;
	weight = w;
}


AStarNode_Dynamic::AStarNode_Dynamic(ushort id, const glm::vec3& pos) 
	: ID(id), position(pos), numConnections(0) {}
AStarNode_Dynamic::AStarNode_Dynamic(const AStarNode_Static& node)
{
	ID = node.ID;
	position = node.position;
	numConnections = node.numConnections;

	for(uint c = 0; c < node.numConnections; c++)
	{
		connections.push_back(AStarConnection(node.connections[c].connectorID, node.connections[c].weight));
	}
}
void AStarNode_Dynamic::AddConnection(ushort cID, float weight)
{
	// dont allow duplicate connections
	for(uint i = 0; i < connections.size(); i++)
	{
		if(connections[i].connectorID == cID)
			return;
	}

	connections.push_back(AStarConnection(cID, weight));
	numConnections++;
}
void AStarNode_Dynamic::AddConnection(const AStarNode_Dynamic& node)
{
	// dont allow duplicate connections
	for(uint i = 0; i < connections.size(); i++)
	{
		if(connections[i].connectorID == node.ID)
			return;
	}

	float weight = glm::length(position - node.position);
	connections.push_back(AStarConnection(node.ID, weight));
	numConnections++;
}
void AStarNode_Dynamic::RemoveConnection(ushort cID)
{
	for(uint i = 0; i < connections.size(); i++)
	{
		if(connections[i].connectorID == cID)
		{
			connections.erase(connections.begin() + i);
			numConnections--;
			return;		// erase only the first connection with the right ID. (duplicates shouldn't exist anyways)
		}
	}
}
void AStarNode_Dynamic::RemoveConnection(const AStarNode_Dynamic& node)
{
	for(uint i = 0; i < connections.size(); i++)
	{
		if(connections[i].connectorID == node.ID)
		{
			connections.erase(connections.begin() + i);
			numConnections--;
			return;		// erase only the first connection with the right ID. (duplicates shouldn't exist anyways)
		}
	}
}

AStarNode_Static::AStarNode_Static(ushort id, uint numconnections, const glm::vec3& pos)
{
	ID = id;
	position = pos;
	numConnections = numconnections;
	connections = new AStarConnection[numConnections];
}
AStarNode_Static::AStarNode_Static(const AStarNode_Static& node)
{
	ID = node.ID;
	position = node.position;
	numConnections = node.numConnections;

	connections = new AStarConnection[numConnections];
	for(uint c = 0; c < numConnections; c++)
	{
		connections[c] = AStarConnection(node.connections[c].connectorID, node.connections[c].weight);
	}
}
AStarNode_Static::AStarNode_Static(const AStarNode_Dynamic& node)
{
	ID = node.ID;
	position = node.position;
	numConnections = node.numConnections;

	connections = new AStarConnection[numConnections];
	for(uint c = 0; c < numConnections; c++)
	{
		connections[c] = AStarConnection(node.connections[c].connectorID, node.connections[c].weight);
	}
}
AStarNode_Static& AStarNode_Static::operator=(const AStarNode_Static& other)
{
	if(this == &other) return *this;

	ID = other.ID;
	position = other.position;
	numConnections = other.numConnections;

	connections = new AStarConnection[numConnections];
	for(uint c = 0; c < numConnections; c++)
	{
		connections[c] = AStarConnection(other.connections[c].connectorID, other.connections[c].weight);
	}

	return *this;
}
void AStarNode_Static::SetConnection(uint cIndex, ushort cID, float w)
{
	Assert(cIndex < numConnections);

	connections[cIndex] = AStarConnection(cID, w);
}

AStarPath AStarPath::FindPath(AStarNode_Static* nodes, uint startIndex, uint endIndex)
{
	// nodes[startIndex] = where we start from
	// nodes[endIndex] = target node

	// cache the current node (as the start node)
	//	-	iterate throug the current node's connections
	//	-	-	get the cost to the end node
	//	-	-	-	(the cost-so-far + the estimated cost to the end)
	//	-	-	

	std::vector<AStarPath_NodeRecord> openList;
	std::vector<AStarPath_NodeRecord> closedList;

	AStarPath_NodeRecord startRecord;
	startRecord.connection = &nodes[startIndex];
	startRecord.node = &nodes[startIndex];
	startRecord.costSoFar = 0;
	startRecord.estimatedTotalCost = glm::length(nodes[endIndex].position - nodes[startIndex].position);

	openList.push_back(startRecord);
	uint currentRecordIndex = 0;

	while(openList.size() > 0)
	{
		// if we're at the end node, break.
		if(openList[currentRecordIndex].node->ID == endIndex) {  break; }

		AStarNode_Static* currentNode = openList[currentRecordIndex].node;
		
		#pragma region CHECK EACH CONNECTION
		// for each connection.
		for(uint c = 0; c < currentNode->numConnections; c++)
		{
			uint connectionID = currentNode->connections[c].connectorID;
			bool recordFound = false;

			// look for the connection on the open list
			for(uint oli = 0; oli < openList.size(); oli++)
			{
				if(openList[oli].node == &nodes[connectionID])
				{
					recordFound = true;

					// check if this iteration would have a lower cost-so-far than previously cached. if yes, update with the new path.
					float newCostSoFar = openList[currentRecordIndex].costSoFar + glm::length(nodes[connectionID].position - currentNode->position);
					if(newCostSoFar < openList[oli].costSoFar)
					{
						openList[oli].costSoFar = newCostSoFar;
						openList[oli].connection = currentNode;
						openList[oli].estimatedTotalCost = newCostSoFar + glm::length(nodes[endIndex].position - nodes[connectionID].position);
					}

					break;
				}
			}
			if(recordFound) continue;
			
			// look for the connection on the closed list
			for(uint cli = 0; cli < closedList.size(); cli++)
			{
				if(closedList[cli].node == &nodes[connectionID])
				{
					recordFound = true;

					// check if this iteration would have a lower cost-so-far than previously cached. if yes, update with the new path.
					float newCostSoFar = openList[currentRecordIndex].costSoFar + glm::length(nodes[connectionID].position - currentNode->position);
					if(newCostSoFar < closedList[cli].costSoFar)
					{
						closedList[cli].costSoFar = newCostSoFar;
						closedList[cli].connection = currentNode;
						closedList[cli].estimatedTotalCost = newCostSoFar + glm::length(nodes[endIndex].position - nodes[connectionID].position);

						// move it back to the open list.
						openList.push_back(closedList[cli]);
						closedList.erase(closedList.begin() + cli);
					}

					break;
				}
			}
			if(recordFound) continue;

			// create the connection struct because it wasn't found, put it on the open list
			{
				AStarPath_NodeRecord newRecord;
				newRecord.node = &nodes[connectionID];
				newRecord.connection = currentNode;
				newRecord.costSoFar = openList[currentRecordIndex].costSoFar + glm::length(newRecord.connection->position - newRecord.node->position);
				newRecord.estimatedTotalCost = newRecord.costSoFar + glm::length(nodes[endIndex].position - newRecord.node->position);

				// add the newly created record to the open list.
				openList.push_back(newRecord);
			}
		}
		#pragma endregion

		// put the current node record on the closed list.
		closedList.push_back(openList[currentRecordIndex]);
		openList.erase(openList.begin() + currentRecordIndex);

		// iterate for the next value on the open list that has the smallest cost-so-far
		float smallestEstimate = FLT_MAX;
		for(uint i = 0; i < openList.size(); i++)
		{
			if(openList[i].estimatedTotalCost < smallestEstimate)
			{
				smallestEstimate = openList[i].estimatedTotalCost;
				currentRecordIndex = i;
			}
		}
	}

	// current  = the end node
	std::vector<uint> reversePathNodes;
	AStarPath_NodeRecord current = openList[currentRecordIndex];
	while(current.node->ID != nodes[startIndex].ID)
	{
		reversePathNodes.push_back(current.node->ID);
		
		for(uint i = 0; i < closedList.size(); i++)
		{
			if(closedList[i].node->ID == current.connection->ID)
			{
				current = closedList[i];
				break;
			}
		}
	}
	reversePathNodes.push_back(nodes[startIndex].ID);

	// convert the vector to an array
	uint* pathNodes = new uint[reversePathNodes.size()];
	for(uint i = 0; i < reversePathNodes.size(); i++)
	{
		pathNodes[i] = reversePathNodes[(reversePathNodes.size() - 1) - i];
	}
	
	return AStarPath(pathNodes, reversePathNodes.size(), 0);
}
AStarPath AStarPath::FindPath(AStarNode_Static* nodes, uint numNodes, glm::vec3 start, glm::vec3 end)
{
	// get index of node closest to the start:
	uint startindex = (uint)-1;
	uint endindex = (uint)-1;

	float shortest_toStart = FLT_MAX;
	float shortest_toEnd = FLT_MAX;
	for(uint i = 0; i < numNodes; i++)
	{
		float val = glm::length(start - nodes[i].position);
		if(val < shortest_toStart)
		{
			shortest_toStart = val;
			startindex = i;
		}

		val = glm::length(end - nodes[i].position);
		if(val < shortest_toEnd)
		{
			shortest_toEnd = val;
			endindex = i;
		}
	}

	return FindPath(nodes, startindex, endindex);
}

AStarPath AStarPath::FindPath(AStarNode_Static* nodes, uint startIndex, uint endIndex, fastdelegate::FastDelegate2<uint, uint, float> heuristic)
{
	// nodes[startIndex] = where we start from
	// nodes[endIndex] = target node

	// cache the current node (as the start node)
	//	-	iterate throug the current node's connections
	//	-	-	get the cost to the end node
	//	-	-	-	(the cost-so-far + the estimated cost to the end)
	//	-	-	

	std::vector<AStarPath_NodeRecord> openList;
	std::vector<AStarPath_NodeRecord> closedList;

	AStarPath_NodeRecord startRecord;
	startRecord.connection = &nodes[startIndex];
	startRecord.node = &nodes[startIndex];
	startRecord.costSoFar = 0;
	startRecord.estimatedTotalCost = glm::length(nodes[endIndex].position - nodes[startIndex].position);

	openList.push_back(startRecord);
	uint currentRecordIndex = 0;

	while(openList.size() > 0)
	{
		// if we're at the end node, break.
		if(openList[currentRecordIndex].node->ID == endIndex) {  break; }

		AStarNode_Static* currentNode = openList[currentRecordIndex].node;
		
		#pragma region CHECK EACH CONNECTION
		// for each connection.
		for(uint c = 0; c < currentNode->numConnections; c++)
		{
			uint connectionID = currentNode->connections[c].connectorID;
			bool recordFound = false;

			// look for the connection on the open list
			for(uint oli = 0; oli < openList.size(); oli++)
			{
				if(openList[oli].node == &nodes[connectionID])
				{
					recordFound = true;

					// check if this iteration would have a lower cost-so-far than previously cached. if yes, update with the new path.
					float newCostSoFar = openList[currentRecordIndex].costSoFar + glm::length(nodes[connectionID].position - currentNode->position);
					if(newCostSoFar < openList[oli].costSoFar)
					{
						openList[oli].costSoFar = newCostSoFar;
						openList[oli].connection = currentNode;
						openList[oli].estimatedTotalCost = newCostSoFar + heuristic(openList[oli].node->ID, endIndex);//glm::length(nodes[endIndex].position - nodes[connectionID].position);
					}

					break;
				}
			}
			if(recordFound) continue;
			
			// look for the connection on the closed list
			for(uint cli = 0; cli < closedList.size(); cli++)
			{
				if(closedList[cli].node == &nodes[connectionID])
				{
					recordFound = true;

					// check if this iteration would have a lower cost-so-far than previously cached. if yes, update with the new path.
					float newCostSoFar = openList[currentRecordIndex].costSoFar + glm::length(nodes[connectionID].position - currentNode->position);
					if(newCostSoFar < closedList[cli].costSoFar)
					{
						closedList[cli].costSoFar = newCostSoFar;
						closedList[cli].connection = currentNode;
						closedList[cli].estimatedTotalCost = newCostSoFar + heuristic(closedList[cli].node->ID, endIndex);//glm::length(nodes[endIndex].position - nodes[connectionID].position);

						// move it back to the open list.
						openList.push_back(closedList[cli]);
						closedList.erase(closedList.begin() + cli);
					}

					break;
				}
			}
			if(recordFound) continue;

			// create the connection struct because it wasn't found, put it on the open list
			{
				AStarPath_NodeRecord newRecord;
				newRecord.node = &nodes[connectionID];
				newRecord.connection = currentNode;
				newRecord.costSoFar = openList[currentRecordIndex].costSoFar + glm::length(newRecord.connection->position - newRecord.node->position);
				newRecord.estimatedTotalCost = newRecord.costSoFar + heuristic(connectionID, endIndex);//glm::length(nodes[endIndex].position - newRecord.node->position);

				// add the newly created record to the open list.
				openList.push_back(newRecord);
			}
		}
		#pragma endregion

		// put the current node record on the closed list.
		closedList.push_back(openList[currentRecordIndex]);
		openList.erase(openList.begin() + currentRecordIndex);

		// iterate for the next value on the open list that has the smallest cost-so-far
		float smallestEstimate = FLT_MAX;
		for(uint i = 0; i < openList.size(); i++)
		{
			if(openList[i].estimatedTotalCost < smallestEstimate)
			{
				smallestEstimate = openList[i].estimatedTotalCost;
				currentRecordIndex = i;
			}
		}
	}

	// current  = the end node
	std::vector<uint> reversePathNodes;
	AStarPath_NodeRecord current = openList[currentRecordIndex];
	while(current.node->ID != nodes[startIndex].ID)
	{
		reversePathNodes.push_back(current.node->ID);
		
		for(uint i = 0; i < closedList.size(); i++)
		{
			if(closedList[i].node->ID == current.connection->ID)
			{
				current = closedList[i];
				break;
			}
		}
	}
	reversePathNodes.push_back(nodes[startIndex].ID);

	// convert the vector to an array
	uint* pathNodes = new uint[reversePathNodes.size()];
	for(uint i = 0; i < reversePathNodes.size(); i++)
	{
		pathNodes[i] = reversePathNodes[(reversePathNodes.size() - 1) - i];
	}
	
	return AStarPath(pathNodes, reversePathNodes.size(), 0);
}
AStarPath AStarPath::FindPath(AStarNode_Static* nodes, uint numNodes, glm::vec3 start, glm::vec3 end, fastdelegate::FastDelegate2<uint, uint, float> heuristic)
{
	uint startindex = (uint)-1;
	uint endindex = (uint)-1;

	float shortest_toStart = FLT_MAX;
	float shortest_toEnd = FLT_MAX;
	for(uint i = 0; i < numNodes; i++)
	{
		float val = glm::length(start - nodes[i].position);
		if(val < shortest_toStart)
		{
			shortest_toStart = val;
			startindex = i;
		}

		val = glm::length(end - nodes[i].position);
		if(val < shortest_toEnd)
		{
			shortest_toEnd = val;
			endindex = i;
		}
	}

	return FindPath(nodes, startindex, endindex, heuristic);
}