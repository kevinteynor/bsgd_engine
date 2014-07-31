#include "DebugShapes.h"

#ifdef USE_DEBUG_SHAPES

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <Qt\qdebug.h>
#include <ShapeData.h>
#include <ShapeGenerator.h>
#define ASSERTS
#include "Assertion.h"
#include "DebugShapeShader.inl"
#include "PerfTimer.h"
#include "Renderer.h"

uint DebugShapes::SHADER_ID = (uint)-1;
uint DebugShapes::LINE_GEO_ID = (uint)-1;
uint DebugShapes::VECTOR_GEO_ID = (uint)-1;
uint DebugShapes::ARROW_GEO_ID = (uint)-1;
uint DebugShapes::PLANE_GEO_ID = (uint)-1;
uint DebugShapes::CUBE_GEO_ID = (uint)-1;
uint DebugShapes::SPHERE_GEO_ID = (uint)-1;
uint DebugShapes::nextInstanceIndex = 0;
DebugShapes::DebugShapeInstanceInfo DebugShapes::shapeInstancesInfo[MAX_DEBUG_RENDERABLES];
std::vector<DebugShapes::DebugShapeInstanceInfo> DebugShapes::shapeInstances;

Renderer* DebugShapes::activeRenderer;

void DebugShapes::SetRenderer(Renderer* const renderer)
{
	activeRenderer = renderer;
}
void DebugShapes::CheckForShader()
{
	if(SHADER_ID != (uint)-1)
		return;

	Assert(activeRenderer != NULL);
	SHADER_ID = activeRenderer->AddShader_CHARARRAY(debug_shapes_vertex_shader_code, debug_shapes_fragment_shader_code);
	//SHADER_ID = activeRenderer->AddShader("d_vert_shader.vsh", "d_frag_shader.fsh");
}

uint DebugShapes::MakeArrow(const glm::mat4& position, const glm::vec4& color, RenderLayer layer, float lifetime)
{
	Assert(activeRenderer != NULL);

	if(ARROW_GEO_ID == (uint)-1)
	{
		Neumont::ShapeData data = Neumont::ShapeGenerator::makeArrow();
		ARROW_GEO_ID = activeRenderer->AddGeometry(data.verts, data.numVerts, data.indices, data.numIndices, rm_TRIANGLES);
	}
	CheckForShader();

	Assert(nextInstanceIndex < MAX_DEBUG_RENDERABLES);

	DebugShapeInstanceInfo newShape = DebugShapeInstanceInfo();
	newShape.duration = lifetime;
	newShape.elapsed = 0;
	newShape.position = position;
	newShape.geoBufferID = ARROW_GEO_ID;
	newShape.color = color;
	newShape.layer = layer;

	return MakeShape(newShape);
}
void DebugShapes::MakeCrosshair(const glm::mat4& position, float lifetime)
{
	Assert(activeRenderer != NULL);
	
	MakeLine(position * glm::scale(-1.0f, 1.0f,1.0f) * glm::translate(-1.0f, 0.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), rl_TOPMOST, lifetime);	// x-axis
	MakeLine(glm::translate(0.0f, 1.0f, 0.0f) * glm::rotate(position, -90.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), rl_TOPMOST, lifetime); // y-axis
	MakeLine(glm::translate(0.0f, 0.0f, 1.0f) * glm::rotate(position, 90.0f, 0.0f, 1.0f, 0.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), rl_TOPMOST, lifetime);	// z-axis
}
uint DebugShapes::MakeCube(const glm::mat4& position, const glm::vec4& color, RenderLayer layer, float lifetime)
{
	Assert(activeRenderer != NULL);

	if(CUBE_GEO_ID == (uint)-1)
	{
		Neumont::ShapeData data = Neumont::ShapeGenerator::makeCube();
		CUBE_GEO_ID = activeRenderer->AddGeometry(data.verts, data.numVerts, data.indices, data.numIndices, rm_TRIANGLES);
	}
	CheckForShader();

	Assert(nextInstanceIndex < MAX_DEBUG_RENDERABLES);

	DebugShapeInstanceInfo newShape = DebugShapeInstanceInfo();
	newShape.duration = lifetime;
	newShape.elapsed = 0;
	newShape.position = position;
	newShape.geoBufferID = CUBE_GEO_ID;
	newShape.color = color;
	newShape.layer = layer;

	return MakeShape(newShape);
}
uint DebugShapes::MakeLine(const glm::mat4& position, const glm::vec4& color, RenderLayer layer, float lifetime)
{
	Assert(activeRenderer != NULL);

	if(LINE_GEO_ID == (uint)-1)
	{
		Neumont::ShapeData data = Neumont::ShapeGenerator::makeLine();
		LINE_GEO_ID = activeRenderer->AddGeometry(data.verts, data.numVerts, data.indices, data.numIndices, rm_LINES);
	}
	CheckForShader();

	Assert(nextInstanceIndex < MAX_DEBUG_RENDERABLES);
	
	DebugShapeInstanceInfo newShape = DebugShapeInstanceInfo();
	newShape.duration = lifetime;
	newShape.elapsed = 0;
	newShape.position = position;
	newShape.geoBufferID = LINE_GEO_ID;
	newShape.color = color;
	newShape.layer = layer;

	return MakeShape(newShape);
}
uint DebugShapes::MakeVector(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color, RenderLayer layer, float lifetime)
{
	Assert(activeRenderer != NULL);

	if(VECTOR_GEO_ID == (uint)-1)
	{
		Neumont::ShapeData data = Neumont::ShapeGenerator::makeVector(5);
		VECTOR_GEO_ID = activeRenderer->AddGeometry(data.verts, data.numVerts, data.indices, data.numIndices, rm_TRIANGLES);
	}
	CheckForShader();

	Assert(nextInstanceIndex < MAX_DEBUG_RENDERABLES);

	DebugShapeInstanceInfo newShape = DebugShapeInstanceInfo();
	newShape.duration = lifetime;
	newShape.elapsed = 0;

	// transform matrix
	glm::vec3 rotation = end - start;
	float length = glm::length(rotation);
	float rotationAngle = acosf(glm::dot(glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)), glm::normalize(rotation)));
	float convertedAngle = rotationAngle * 180.0f / PI;
	
	glm::vec3 cross = glm::cross(glm::vec3(0,0,1), rotation);
	if(end.x == start.x && end.y == start.y)
		cross = glm::cross(glm::vec3(0,1,0), rotation);
	
	if(cross.x == 0.0f && cross.y == 0.0f && cross.z == 0.0f)
	{
		cross.x = 1.0f;
		convertedAngle = 0;
	}
	glm::mat4 transform = glm::translate(start) * 
		glm::rotate(convertedAngle, cross) * 
		glm::scale(1.0f, 1.0f, length) *
		glm::translate(0.0f, -0.015625f, 0.5f) * 
		glm::scale(0.0625f, 0.0625f, -0.5f);
	// transform matrix

	newShape.position = transform;
	newShape.geoBufferID = VECTOR_GEO_ID;
	newShape.color = color;
	newShape.layer = layer;

	return MakeShape(newShape);
}
uint DebugShapes::MakePlane(const glm::mat4& position, const glm::vec4& color, uint dimensions, RenderLayer layer, float lifetime)
{
	Assert(activeRenderer != NULL);

	if(PLANE_GEO_ID == (uint)-1)
	{
		Neumont::ShapeData data = Neumont::ShapeGenerator::makePlane(dimensions, dimensions);
		PLANE_GEO_ID = activeRenderer->AddGeometry(data.verts, data.numVerts, data.indices, data.numIndices, rm_TRIANGLES);
	}
	CheckForShader();

	Assert(nextInstanceIndex < MAX_DEBUG_RENDERABLES);

	DebugShapeInstanceInfo newShape = DebugShapeInstanceInfo();
	newShape.duration = lifetime;
	newShape.elapsed = 0;
	newShape.position = position;
	newShape.geoBufferID = PLANE_GEO_ID;
	newShape.color = color;
	newShape.layer = layer;

	return MakeShape(newShape);	
}
uint DebugShapes::MakeSphere(const glm::mat4& position, const glm::vec4& color, uint tesselation, RenderLayer layer, float lifetime)
{
	Assert(activeRenderer != NULL);

	if(SPHERE_GEO_ID == (uint)-1)
	{
		Neumont::ShapeData data = Neumont::ShapeGenerator::makeSphere(tesselation);
		SPHERE_GEO_ID = activeRenderer->AddGeometry(data.verts, data.numVerts, data.indices, data.numIndices, rm_TRIANGLES);
	}
	CheckForShader();

	Assert(nextInstanceIndex < MAX_DEBUG_RENDERABLES);

	DebugShapeInstanceInfo newShape = DebugShapeInstanceInfo();
	newShape.duration = lifetime;
	newShape.elapsed = 0;
	newShape.position = position;
	newShape.geoBufferID = SPHERE_GEO_ID;
	newShape.color = color;
	newShape.layer = layer;

	return MakeShape(newShape);
}

uint DebugShapes::MakeShape(DebugShapeInstanceInfo& shapeInfo)
{
	shapeInstances.push_back(shapeInfo);
	nextInstanceIndex++;

	shapeInstances[shapeInstances.size() - 1].renderable = activeRenderer->AddRenderableInstance_p(shapeInfo.geoBufferID, shapeInfo.position, SHADER_ID);
	//shapeInstances[shapeInstances.size() - 1].renderable->useDepthTest = shapeInfo.UseDepthTest;
	shapeInstances[shapeInstances.size() - 1].renderable->layer = shapeInfo.layer;
	shapeInstances[shapeInstances.size() - 1].renderable->AddUniform("mvp", ut_MAT4_MVP, 0, 0);
	shapeInstances[shapeInstances.size() - 1].colorUniformIndex = shapeInstances[shapeInstances.size() - 1].renderable->AddUniform("uniColor", ut_VEC4, &shapeInfo.color, sizeof(glm::vec4));
	return shapeInstances.size() - 1;
}
void DebugShapes::RemoveShape(uint index)
{
	Assert(index >= 0 && index < shapeInstances.size());

	shapeInstances[index].renderable->visible = false;
	shapeInstances.erase(shapeInstances.begin() + index);

	nextInstanceIndex--;
}

void DebugShapes::Update(float dt)
{
	for(int i = shapeInstances.size() - 1; i >= 0; i--)
	{
		// check if elapsed time is greater than the lifetime.
		shapeInstances[i].elapsed += dt;

		if(shapeInstances[i].elapsed > shapeInstances[i].duration && shapeInstances[i].duration != (uint)-1)
			RemoveShape(i);
	}
}

void DebugShapes::SetShapeColor(uint shapeID, const glm::vec3& color)
{
	Assert(shapeID < shapeInstances.size());

	shapeInstances[shapeID].SetColor(color);
}
void DebugShapes::SetShapeColor(uint shapeID, const glm::vec4& color)
{
	Assert(shapeID < shapeInstances.size());

	shapeInstances[shapeID].SetColor(color);
}
void DebugShapes::SetShapeTransform(uint shapeID, const glm::mat4& trans)
{
	Assert(shapeID < shapeInstances.size());

	shapeInstances[shapeID].SetTransform(trans);

}
void DebugShapes::SetShapeVisibility(uint shapeID, const bool& visible)
{
	Assert(shapeID < shapeInstances.size());

	shapeInstances[shapeID].renderable->visible = visible;
}
void DebugShapes::SetShapeLayer(uint shapeID, const RenderLayer& layer)
{
	Assert(shapeID < shapeInstances.size());

	shapeInstances[shapeID].renderable->layer = layer;
}

glm::mat4& DebugShapes::GetShapeTransform(uint shapeID)
{
	Assert(shapeID < shapeInstances.size());

	return shapeInstances[shapeID].position;
}

void DebugShapes::DebugShapeInstanceInfo::SetColor(glm::vec3 c)
{
	color.r = c.r;
	color.g = c.g;
	color.b = c.b;
	renderable->UpdateUniform(colorUniformIndex, &color, sizeof(glm::vec3));
}
void DebugShapes::DebugShapeInstanceInfo::SetColor(glm::vec4 c)
{
	color = c;
	renderable->UpdateUniform(colorUniformIndex, &color, sizeof(glm::vec4));
}
void DebugShapes::DebugShapeInstanceInfo::SetTransform(glm::mat4 trans)
{
	position = trans;
	renderable->position = trans;
}

#endif // USE_DEBUG_SHAPES