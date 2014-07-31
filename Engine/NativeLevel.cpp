#include "NativeLevel.h"

#include <fstream>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <iostream>
#include <ShapeData.h>
#include <Vertex.h>
#include "AStarPath.h"
#include "EngineTypeDefs.h"
#include "NativeModel.h"
#include "RenderableData.h"
#include "Renderer.h"

void NativeLevel_Dynamic::AddPathingNode(const glm::vec3& p)
{
	nodes.push_back(AStarNode_Dynamic(nextNodeID, glm::vec3(p.x, p.y, p.z)));
	nextNodeID++;
}
void NativeLevel_Dynamic::RemovePathingNode(const uint& ID)
{
	nodes.erase(nodes.begin() + ID);
}

NativeLevel_Dynamic NativeLevel_Dynamic::ReadFile(const char* filepath, Renderer* renderer, uint shader)
{
	// instantiate a NativeLevel to load the file data into.
	NativeLevel_Dynamic level;

	std::ifstream in(filepath, std::ios::in | std::ios::binary);

	// read the header length, initialize the segment offset array.
	char* buffer = (char*)malloc(4);
	in.read(buffer, 4);
	uint HEADER_LENGTH = *reinterpret_cast<uint*>(buffer);
	free(buffer);

	uint NUM_SEGMENTS = HEADER_LENGTH / 4;
	uint* SEGMENT_STARTS = new uint[NUM_SEGMENTS + 1];	// add an extra index that will store the end of the file. this is so segment length can be determined by the SEGMENT_START of the next index.
	
	// read the header, collect the file meta-data
	ReadHeader(&in, SEGMENT_STARTS, HEADER_LENGTH);

	if(NUM_SEGMENTS >= 1)
	{
		// read the renderable data from file, then make sense of it.
		in.seekg(SEGMENT_STARTS[0]);
		char* renderables_buffer = (char*)malloc(SEGMENT_STARTS[1] - SEGMENT_STARTS[0]);
		in.read(renderables_buffer, SEGMENT_STARTS[1] - SEGMENT_STARTS[0]);
		ReadRenderablesData(renderables_buffer, level, renderer, shader);
		free(renderables_buffer);
	}
	if(NUM_SEGMENTS >= 2)
	{
		// read the data in the file and make sense of the pathing data.
		char* buffer = (char*)malloc(SEGMENT_STARTS[NUM_SEGMENTS] - SEGMENT_STARTS[1]);
		in.read(buffer, SEGMENT_STARTS[2] - SEGMENT_STARTS[1]);
		ReadPathingNodesData(buffer, level);
		free(buffer);
	}
	// when more things are added to the file, read them in here.

	in.close();
	delete [] SEGMENT_STARTS;
	SEGMENT_STARTS = 0;

	return level;
}
void NativeLevel_Dynamic::ReadHeader(std::ifstream* stream, uint* SEGMENT_STARTS, uint& HEADER_LENGTH)
{
	// read the header data
	stream->seekg(4, std::ios::beg);
	char* buffer = (char*)malloc(HEADER_LENGTH);
	stream->read(buffer, HEADER_LENGTH);

	// set the segments values from the file data
	memcpy(SEGMENT_STARTS, buffer, HEADER_LENGTH);

	// set the last SEGMENT_START to the end of the file.
	stream->seekg(0, std::ios::end);
	//SEGMENT_STARTS[NUM_SEGMENTS] = stream->tellg();
	//uint endPos = stream->tellg();
	SEGMENT_STARTS[HEADER_LENGTH / 4] = stream->tellg();

	free(buffer);

}
void NativeLevel_Dynamic::ReadRenderablesData(char* data, NativeLevel_Dynamic& level, Renderer* renderer, uint shader)
{
	// read how many renderables there are
	uint RENDERABLE_COUNT = *reinterpret_cast<uint*>(data);

	// setup array to hold the renderable data
	RenderableData* renderableData = new RenderableData[RENDERABLE_COUNT];
	memcpy(renderableData, data + 4, RENDERABLE_COUNT * sizeof(RenderableData));
		
	for(uint i = 0; i < RENDERABLE_COUNT; i++)
	{
		// fix up the pointers for the renderable data's verts/indices
		uint vertexOffset = reinterpret_cast<uint>(renderableData[i].vertices);
		renderableData[i].vertices = new Neumont::Vertex[renderableData[i].numVertices];
		memcpy(renderableData[i].vertices, data + vertexOffset, renderableData[i].numVertices * sizeof(Neumont::Vertex));

		uint indexOffset = reinterpret_cast<uint>(renderableData[i].indices);
		renderableData[i].indices = new ushort[renderableData[i].numIndices];
		memcpy(renderableData[i].indices, data + indexOffset, renderableData[i].numIndices * sizeof(ushort));

		// change the RenderableData into something consumable by the NativeLevel and OpenGL.
		uint geoID = renderer->AddGeometry(renderableData[i].vertices, renderableData[i].numVertices, renderableData[i].indices, renderableData[i].numIndices, rm_TRIANGLES);
		level.renderables.push_back(new Renderer::RenderableInstanceInfo());
		level.renderables[level.renderables.size() - 1] = renderer->AddRenderableInstance_p(geoID, renderableData[i].transform, shader);
	}

	delete [] renderableData;
	renderableData = 0;
}
void NativeLevel_Dynamic::ReadPathingNodesData(char* data, NativeLevel_Dynamic& level)
{
	// read how many nodes there are
	uint NODE_COUNT = *reinterpret_cast<uint*>(data);

	// setup array to hold the node data
	AStarNode_Static* nodeData = new AStarNode_Static[NODE_COUNT];
	memcpy(nodeData, data + 4, NODE_COUNT * sizeof(AStarNode_Static));
	
	for(uint i = 0; i < NODE_COUNT; i++)
	{
		uint connectionOffset = reinterpret_cast<uint>(nodeData[i].connections);
		nodeData[i].connections = new AStarConnection[nodeData[i].numConnections];
		memcpy(nodeData[i].connections, data + connectionOffset, nodeData[i].numConnections * sizeof(AStarConnection));

		level.nodes.push_back(AStarNode_Dynamic(nodeData[i]));
	}

	level.nextNodeID = NODE_COUNT;

	// WHY CANT I DELETE THIS!?!
//	delete nodeData;
	nodeData = 0;
}

void NativeLevel_Dynamic::WriteFile(const char* filepath)
{
	std::ofstream out(filepath, std::ios::out | std::ios::binary);
	uint HEADER_PREFIX_LENGTH = 4;
	uint HEADER_LENGTH = 8;
	uint* SEGMENT_STARTS = new uint[2];

	SEGMENT_STARTS[0] = HEADER_PREFIX_LENGTH + HEADER_LENGTH;
	WriteRenderablesData(&out, SEGMENT_STARTS[0]);

	out.seekp(0, std::ios::end);
	SEGMENT_STARTS[1] = out.tellp();
	WritePathingNodesData(&out, SEGMENT_STARTS[1]);

	WriteHeaderInfo(&out, HEADER_LENGTH, SEGMENT_STARTS);

	out.close();
	delete [] SEGMENT_STARTS;
	SEGMENT_STARTS = 0;
}
void NativeLevel_Dynamic::WriteHeaderInfo(std::ofstream* stream, uint& HEADER_LENGTH, uint* SEGMENT_STARTS)
{
	// Header Info:
	// [header prefix = 4bytes][header data = ???bytes]

	stream->seekp(0, std::ios::beg);

	// [####][....][....]		header prefix
	stream->write(reinterpret_cast<char*>(&HEADER_LENGTH), 4);

	// [****][####][....]		renderables
	stream->write(reinterpret_cast<char*>(&SEGMENT_STARTS[0]), 4);			// MAKE SURE THIS ACTUALLY WORKS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	// [****][****][####]		pathing nodes
	stream->write(reinterpret_cast<char*>(&SEGMENT_STARTS[1]), 4);		// AND BY THIS, I MEAN "&SEGMENT_STARTS + 1" !!!!!!!!!!!!!!!!!!!!!!!!!!
}
void NativeLevel_Dynamic::WriteRenderablesData(std::ofstream* stream, const uint& SEGMENT_START)
{
	// [count][shape datas][vertex/index data]
	// [  4  ][#sd  * size][        ???      ]

	stream->seekp(SEGMENT_START, std::ios::beg);

	uint RENDERABLE_COUNT = renderables.size();
	stream->write(reinterpret_cast<char*>(&RENDERABLE_COUNT), 4);

	// set up array of RenderableData to make reading/writing the needed parts easier.
	RenderableData* renderableData = new RenderableData[RENDERABLE_COUNT];

	// seek to the vertex/index part of the buffer (needs to be written before the instances so we can fix the pointers)
	stream->seekp(SEGMENT_START + 4 + (RENDERABLE_COUNT * sizeof(RenderableData)), std::ios::beg);

	for(uint i = 0; i < RENDERABLE_COUNT; i++)
	{
		RenderableData& d = renderableData[i];

		// TRANSFORM DATA:
		d.transform = renderables[i]->position;

		// VERTEX DATA:
		void* vertexData = malloc(renderables[i]->geometry->vertexBufferSize);
		glGetBufferSubData(GL_ARRAY_BUFFER, renderables[i]->geometry->vertexBufferOffset, renderables[i]->geometry->vertexBufferSize, vertexData);
		d.numVertices = renderables[i]->geometry->vertexCount;
		d.vertices = reinterpret_cast<Neumont::Vertex*>(vertexData);

		// INDEX DATA:
		void* indexData = malloc(renderables[i]->geometry->indexBufferSize);
		glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, renderables[i]->geometry->indexBufferOffset, renderables[i]->geometry->indexBufferSize, indexData);
		d.numIndices = renderables[i]->geometry->indexCount;
		d.indices = reinterpret_cast<ushort*>(indexData);
		
		// WRITE THE DATA/FIX DATA POINTERS:
		uint dataFileOffset = stream->tellp();	// cache the offset into the file. 
		dataFileOffset -= SEGMENT_START;		// make it relative to this segment.
		stream->write(reinterpret_cast<char*>(d.vertices), d.vertexBufferSize());	// write out the vertex data
		memcpy(&d.vertices, &dataFileOffset, sizeof(Neumont::Vertex*));	// set the RenderableData's vertex pointer to the data's offset into the file

		dataFileOffset = stream->tellp();
		dataFileOffset -= SEGMENT_START;
		stream->write(reinterpret_cast<char*>(d.indices), d.indexBufferSize());
		memcpy(&d.indices, &dataFileOffset, sizeof(ushort*));

		// cleanup
		free(vertexData);
		free(indexData);
	}

	// seek to where the renderable data objects will be written
	stream->seekp(SEGMENT_START + 4, std::ios::beg);
	
	// write the renderable data objects
	stream->write(reinterpret_cast<char*>(renderableData), RENDERABLE_COUNT * sizeof(RenderableData));

	// clean up
	delete [] renderableData;
	renderableData = 0;
}
void NativeLevel_Dynamic::WritePathingNodesData(std::ofstream* stream, const uint& SEGMENT_START)
{
	stream->seekp(SEGMENT_START, std::ios::beg);

	uint NODE_COUNT = nodes.size();
	stream->write(reinterpret_cast<char*>(&NODE_COUNT), 4);	// write out the # of nodes

	// Duplicate the dynamic nodes as static ones for writing to the file.
	AStarNode_Static* nodes_ToWrite = new AStarNode_Static[NODE_COUNT];
	for(uint i = 0; i < NODE_COUNT; i++)
	{
		nodes_ToWrite[i] = AStarNode_Static(nodes[i]);
	}

	// Seek to the node-connections location (these need to be written first)
	stream->seekp(SEGMENT_START + 4 + (NODE_COUNT * sizeof(AStarNode_Static)), std::ios::beg);

	for(uint i = 0; i < NODE_COUNT; i++)
	{
		// cache this node's connection relative to the start of the segment.
		uint connectionsOffset = stream->tellp();
		connectionsOffset -= SEGMENT_START;

		stream->write(reinterpret_cast<char*>(nodes_ToWrite[i].connections), nodes_ToWrite[i].numConnections * sizeof(AStarConnection));

		memcpy(&nodes_ToWrite[i].connections, &connectionsOffset, 4);	// fix up the node's connection
	}

	// Seek to the node data locations (can now be written after doing the fix-up on the node-connections
	stream->seekp(SEGMENT_START + 4, std::ios::beg);
	// write each node to file
	for(uint i = 0; i < NODE_COUNT; i++)
	{
		stream->write(reinterpret_cast<char*>(&nodes_ToWrite[i]), sizeof(AStarNode_Static));
	}

	delete [] nodes_ToWrite;
	nodes_ToWrite = 0;
}

NativeLevel_Dynamic NativeLevel_Dynamic::Load(const char* filepath, Renderer* renderer, uint shader)
{
	return ReadFile(filepath, renderer, shader);
}
void NativeLevel_Dynamic::Save(const char* filepath)
{
	WriteFile(filepath);
}
 
// ==========================================================================
NativeLevel_Static NativeLevel_Static::ReadFile(const char* filepath, Renderer* renderer, uint shader)
{
	NativeLevel_Static level;

	std::ifstream in(filepath, std::ios::in | std::ios::binary);

	// read the header length, initialize the segment offset array.
	char* buffer = (char*)malloc(4);
	in.read(buffer, 4);
	uint HEADER_LENGTH = *reinterpret_cast<uint*>(buffer);
	free(buffer);

	uint NUM_SEGMENTS = HEADER_LENGTH / 4;
	uint* SEGMENT_STARTS = new uint[NUM_SEGMENTS + 1];	// add the extra index that will store the EOF.

	// read the header
	ReadHeader(&in, SEGMENT_STARTS, HEADER_LENGTH);

	if(NUM_SEGMENTS >= 1)
	{
		// read the renderable data from file, then make sense of it.
		in.seekg(SEGMENT_STARTS[0]);
		char* renderables_buffer = (char*)malloc(SEGMENT_STARTS[1] - SEGMENT_STARTS[0]);
		in.read(renderables_buffer, SEGMENT_STARTS[1] - SEGMENT_STARTS[0]);
		ReadRenderablesData(renderables_buffer, level, renderer, shader);
		free(renderables_buffer);
	}
	if(NUM_SEGMENTS >= 2)
	{
		char* buffer = (char*)malloc(SEGMENT_STARTS[NUM_SEGMENTS] - SEGMENT_STARTS[1]);
		in.read(buffer, SEGMENT_STARTS[2] - SEGMENT_STARTS[1]);
		ReadPathingNodesData(buffer, level);

		free(buffer);
	}

	in.close();
	delete[] SEGMENT_STARTS;
	SEGMENT_STARTS = 0;

	return level;
}
void NativeLevel_Static::ReadHeader(std::ifstream* stream, uint* SEGMENT_STARTS, uint& HEADER_LENGTH)
{
	// read the header data
	stream->seekg(4, std::ios::beg);
	char* buffer = (char*)malloc(HEADER_LENGTH);
	stream->read(buffer, HEADER_LENGTH);

	// set the segments values from the file data
	memcpy(SEGMENT_STARTS, buffer, HEADER_LENGTH);

	// set the last SEGMENT_START to the end of the file.
	stream->seekg(0, std::ios::end);
	SEGMENT_STARTS[HEADER_LENGTH / 4] = stream->tellg();

	free(buffer);
}
void NativeLevel_Static::ReadRenderablesData(char* data, NativeLevel_Static& level, Renderer* renderer, uint shader)
{
	// read how many renderables there are
	uint RENDERABLE_COUNT = *reinterpret_cast<uint*>(data);

	// setup array to hold the renderable data
	RenderableData* renderableData = new RenderableData[RENDERABLE_COUNT];
	memcpy(renderableData, data + 4, RENDERABLE_COUNT * sizeof(RenderableData));
	level.renderables = new Renderer::RenderableInstanceInfo*[RENDERABLE_COUNT];

	level.numRenderables = RENDERABLE_COUNT;

	for(uint i = 0; i < RENDERABLE_COUNT; i++)
	{
		// fix up the pointers for the renderable data's verst/indices
		uint vertexOffset = reinterpret_cast<uint>(renderableData[i].vertices);
		renderableData[i].vertices = new Neumont::Vertex[renderableData[i].numVertices];
		memcpy(renderableData[i].vertices, data + vertexOffset, renderableData[i].numVertices * sizeof(Neumont::Vertex));

		uint indexOffset = reinterpret_cast<uint>(renderableData[i].indices);
		renderableData[i].indices = new ushort[renderableData[i].numIndices];
		memcpy(renderableData[i].indices, data + indexOffset, renderableData[i].numIndices * sizeof(ushort));

		// change the RenderableData into something consumable by the NativeLevel and OpenGL
		uint geoID = renderer->AddGeometry(renderableData[i].vertices, renderableData[i].numVertices, renderableData[i].indices, renderableData[i].numIndices, rm_TRIANGLES);
		level.renderables[i] = renderer->AddRenderableInstance_p(geoID, renderableData[i].transform, shader);
	}
}
void NativeLevel_Static::ReadPathingNodesData(char* data, NativeLevel_Static& level)
{
	// read how many nodes there are
	uint NODE_COUNT = *reinterpret_cast<uint*>(data);

	level.nodes = new AStarNode_Static[NODE_COUNT];
	memcpy(level.nodes, data + 4, NODE_COUNT * sizeof(AStarNode_Static));

	level.numNodes = NODE_COUNT;

	for(uint i = 0; i < NODE_COUNT; i++)
	{
		uint connectionOffset = reinterpret_cast<uint>(level.nodes[i].connections);
		level.nodes[i].connections = new AStarConnection[level.nodes[i].numConnections];
		memcpy(level.nodes[i].connections, data + connectionOffset, level.nodes[i].numConnections * sizeof(AStarConnection));
	}
}

NativeLevel_Static NativeLevel_Static::Load(const char* filepath, Renderer* renderer, uint shader)
{
	return ReadFile(filepath, renderer, shader);
}