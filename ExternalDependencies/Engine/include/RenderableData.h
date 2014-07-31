#ifndef RENDERABLE_DATA_H
#define RENDERABLE_DATA_H

#include <glm\glm.hpp>
#include <Vertex.h>
#include "EngineTypeDefs.h"

struct ENGINE_SHARED RenderableData
{
	Neumont::Vertex* vertices;
	ushort* indices;

	uint numVertices;
	uint numIndices;

	glm::mat4 transform;

	RenderableData() : vertices(0), indices(0), numVertices(0), numIndices(0), transform(0) {}
	void CleanUp()
	{
		delete [] vertices;
		delete [] indices;

		vertices = 0;
		indices = 0;

		numVertices = numIndices = 0;
	}

	uint vertexBufferSize() const { return numVertices * sizeof(Neumont::Vertex); }
	uint indexBufferSize() const { return numIndices * sizeof(ushort); }
};

#endif // RENDERABLE_DATA_H