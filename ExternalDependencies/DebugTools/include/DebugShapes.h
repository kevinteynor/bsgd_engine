#ifndef DEBUG_SHAPES_H
#define DEBUG_SHAPES_H

#include <glm\glm.hpp>
#include <vector>
#include "EngineTypeDefs.h"
#include "ExportHeader.h"
#include "Renderer.h"
#include "TypeDefs.h"

#ifdef USE_DEBUG_SHAPES

class DEBUG_SHARED DebugShapes
{
public:
	struct DEBUG_SHARED DebugShapeInstanceInfo
	{
		uint geoBufferID;
		float duration;
		float elapsed;
		Renderer::RenderableInstanceInfo* renderable;
		uint colorUniformIndex;
		RenderLayer layer;
		glm::vec4 color;
		glm::mat4 position;

		void SetColor(glm::vec3);
		void SetColor(glm::vec4);
		void SetTransform(glm::mat4);
	};

private:
	DebugShapes(void){}
	~DebugShapes(void){}
	DebugShapes& operator=(const DebugShapes&);

	static uint SHADER_ID;
	static uint ARROW_GEO_ID;
	static uint CUBE_GEO_ID;
	static uint LINE_GEO_ID;
	static uint VECTOR_GEO_ID;
	static uint PLANE_GEO_ID;
	static uint SPHERE_GEO_ID;
	
	static Renderer* activeRenderer;
	static const int MAX_DEBUG_RENDERABLES = 500;
	static uint nextInstanceIndex;
	static DebugShapeInstanceInfo shapeInstancesInfo[MAX_DEBUG_RENDERABLES];
	
	static std::vector<DebugShapeInstanceInfo> shapeInstances;

	static void CheckForShader();
	static uint MakeShape(DebugShapeInstanceInfo& shapeInfo);

public:
	static void SetRenderer(Renderer* const renderer);

	static uint MakeArrow(const glm::mat4& position, const glm::vec4& color = glm::vec4(1.0f,0.0f,0.0f,1.0f), RenderLayer layer = rl_OPAQUE, float lifetime = -1);
	static void MakeCrosshair(const glm::mat4& position, float lifetime = -1);
	static uint MakeCube(const glm::mat4& position, const glm::vec4& color = glm::vec4(1.0f,0.0f,0.0f,1.0f), RenderLayer layer = rl_OPAQUE, float lifetime = -1);
	static uint MakeLine(const glm::mat4& position, const glm::vec4& color = glm::vec4(1.0f,0.0f,0.0f,1.0f), RenderLayer layer = rl_OPAQUE, float lifetime = -1);
	static uint MakeVector(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color = glm::vec4(1.0f, 0.0f, 0.0f,1.0f), RenderLayer layer = rl_OPAQUE, float lifetime = -1);
	static uint MakePlane(const glm::mat4& position, const glm::vec4& color = glm::vec4(1.0f,0.0f,0.0f,1.0f), uint dimensions = 10, RenderLayer layer = rl_OPAQUE, float lifetime = -1);
	static uint MakeSphere(const glm::mat4& position, const glm::vec4& color = glm::vec4(1.0f,0.0f,0.0f,1.0f), uint tesselation = 10, RenderLayer layer = rl_OPAQUE, float lifetime = -1);

	static void RemoveShape(uint shapeID);

	static void SetShapeTransform(uint shapeID, const glm::mat4& trans);
	static void SetShapeColor(uint shapeID, const glm::vec3& color);
	static void SetShapeColor(uint shapeID, const glm::vec4& color);
	static void SetShapeVisibility(uint shapeID, const bool& visible);
	static void SetShapeLayer(uint shapeID, const RenderLayer& layer);
	static glm::mat4& GetShapeTransform(uint shapeID);

	static void Update(float dt);
};

#else

#pragma warning( disable : 4100)

class DEBUG_SHARED DebugShapes
{
private:
	DebugShapes(void);
	~DebugShapes(void);
	DebugShapes& operator=(const DebugShapes&);

	static DebugShapes* localInstance;

public:
	inline static void SetRenderer(Renderer* const renderer){}

	inline static void MakeArrow(const glm::mat4& position, float lifetime = -1, bool depthTest = true, const glm::vec3& color = glm::vec3(0.0f,0.0f,0.0f)){}
	inline static void MakeCrosshair(const glm::mat4& position, float lifetime = -1, bool depthTest = true){}
	inline static void MakeCube(const glm::mat4& position, float lifetime = -1, bool depthTest = true, const glm::vec3& color = glm::vec3(0.0f,0.0f,0.0f)){}
	inline static void MakeLine(const glm::mat4& position, float lifetime = -1, bool depthTest = true, const glm::vec3& color = glm::vec3(0.0f,0.0f,0.0f)){}
	inline static DebugShapeInstanceInfo* MakeVector(const glm::vec3& start, const glm::vec3& end, float lifetime = -1, bool depthTest = true, const glm::vec3& color = glm::vec3(1.0f, 0.0f, 0.0f));
	inline static void MakePlane(const glm::mat4& position, float lifetime = -1, bool depthTest = true, const glm::vec3& color = glm::vec3(0.0f,0.0f,0.0f), uint dimensions = 10){}
	inline static void MakeSphere(const glm::mat4& position, float lifetime = -1, bool depthTest = true, const glm::vec3& color = glm::vec3(0.0f,0.0f,0.0f), uint tesselation = 10){}
	
	inline static void Update(float dt){dt;}

};
#endif // USE_DEBUG_SHAPES

#endif // DEBUG_SHAPES_H