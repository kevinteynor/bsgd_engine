#ifndef RENDERER_H
#define RENDERER_H

#include <glm\glm.hpp>
#include <QtOpenGL\qglwidget>
#include "DebugCamera.h"
#include "EngineTypeDefs.h"
#include "ShaderManager.h"

//#define BUFFER_SIZE 1048576
#define BUFFER_SIZE 16777261
#define MAX_UNIFORM_DATA_SIZE 64

// TODO: move this enum into ShaderInfo.h
enum UniformType
{
	ut_NULL,
	ut_FLOAT,
	ut_INT,
	ut_VEC2,
	ut_VEC3,
	ut_VEC4,
	ut_MAT3,
	ut_MAT4,
	ut_SAMPLER,
	ut_TEXTURE,
	ut_CUBE_MAP,	
	ut_MAT4_MVP,
	ut_MAT4_Model,
	ut_MAT4_View,
	ut_MAT4_Projection
};
enum RenderMode
{
	rm_LINES,
	rm_TRIANGLES
};
enum RenderLayer
{
	// TODO: decide if a mask is a good idea for the layers-- will objects ever need to be on multiple layers?
	rl_OPAQUE,
	rl_TRANSPARENT,
	rl_TOPMOST
};


class ENGINE_SHARED Renderer : public QGLWidget
{
	Q_OBJECT

public:
	struct ENGINE_SHARED GeometryInfo
	{
		uint vertexBufferOffset;
		uint vertexBufferSize;
		uint vertexCount;
		uint vertexBufferID;

		uint indexBufferOffset;
		uint indexBufferSize;
		uint indexCount;
		uint indexBufferID;

		RenderMode renderMode;
	};
	struct ENGINE_SHARED ShaderUniform
	{
		UniformType type;
		char name[MAX_UNIFORM_DATA_SIZE];
		char data[MAX_UNIFORM_DATA_SIZE];

		void Initialize(const char* n, UniformType t, void* d, uint dataSize);	// TODO: auto-detect data size from the enum type.
	};
	struct ENGINE_SHARED ShaderInfo
	{
		uint programID;
		uint fragmentShaderID, vertexShaderID;

		// TODO: save the uniform/layout locations for everything this shader needs
	};
	struct ENGINE_SHARED RenderableInstanceInfo
	{
	private:
		static const uint MAX_UNIFORMS_PER_SHADER = 24;

	public:
		// TODO: make the position just another Uniform.
		// bool useDepthTest;
		glm::mat4 position;	// where
		GeometryInfo* geometry;	// what
		ShaderInfo* shader;		// how
		uint uniformsCount;
		RenderLayer layer;
		ShaderUniform uniforms[MAX_UNIFORMS_PER_SHADER];

		bool visible;

		//RenderableInstanceInfo() : useDepthTest(true), visible(true), uniformsCount(0) {}
		RenderableInstanceInfo() : layer(rl_OPAQUE), visible(true), uniformsCount(0) {}
		~RenderableInstanceInfo() { geometry = 0; shader = 0; }

		uint AddUniform(const char* n, UniformType t, void* d, uint dataSize);
		void UpdateUniform(uint index, void* d, uint dataSize);
	};

	bool RendererInitialized;


	GLuint normalMapTexture;
private:
	uint shadowFrameBufferID;
	uint shadowVertexShaderID;
	uint shadowFragmentShaderID;
	uint shadowProgramID;
	GLuint shadowMapTexture;

	static const uint MAX_NUM_RENDERABLES = 512;
	uint nextRenderableInstanceIndex;
	RenderableInstanceInfo renderableInstances[MAX_NUM_RENDERABLES];

	static const uint MAX_NUM_GEOMETRIES = 32;
	uint nextGeometryIndex;
	GeometryInfo geometries[MAX_NUM_GEOMETRIES];

	static const uint MAX_NUM_SHADERS = 32;
	uint nextShaderIndex;
	ShaderInfo shaders[MAX_NUM_SHADERS];

	// -------------------------------------------

	static const uint MAX_VERTEX_BUFFERS = 10;
	uint activatedVertexBuffers;
	uint currentVertexBufferOffset[MAX_VERTEX_BUFFERS];
	uint vertexIDs[MAX_VERTEX_BUFFERS];

	static const uint MAX_INDEX_BUFFERS = 10;
	uint activatedIndexBuffers;
	uint currentIndexBufferOffset[MAX_INDEX_BUFFERS];
	uint indexIDs[MAX_INDEX_BUFFERS];

	// -------------------------------------------

	glm::vec2 lastMousePos;
	

	void AddGeometryToGLVertexBuffer(GeometryInfo* geometry, void* vertices);
	void AddGeometryToGLIndexBuffer(GeometryInfo* geometry, ushort* indices);
	void UpdateGeometryInGLVertexBuffer(GeometryInfo* geometry, void* vertices);
	void UpdateGeometryInGLIndexBuffer(GeometryInfo* geometry, ushort* indices);
	void ActivateGLVertexBuffer();
	void ActivateGLIndexBuffer();

public:
	Renderer(void);
	~Renderer(void);

	#pragma region RENDERER STATES
	bool SplitView;
	bool CreateShadowMap;


	#pragma endregion

	uint VERTEX_SIZE;
	uint INDEX_SIZE;
	uint POSITION_OFFSET;
	uint COLOR_OFFSET;
	uint NORMAL_OFFSET;
	uint UV_OFFSET;
	uint NORMAL_TANGENT_OFFSET;
	uint NORMAL_BITANGENT_OFFSET;

	glm::mat4 projection;

	glm::vec3 lightPosition;
	glm::vec3 lightDirection;

	DebugCamera camera_dbg;
	DebugCamera camera;

	void RayFromMouseCoords(glm::vec2, glm::vec3*, glm::vec3*);

#pragma region Add/Update Geometry/Instance/Shader
	uint AddGeometry(
		void* vertices,
		uint numVerts,
		ushort* indices,
		uint numIndices, 
		RenderMode mode);

	uint AddShader(
		const char* vertexFile,
		const char* fragmentFile);
	uint AddShader_CHARARRAY(
		const char* vertexString,
		const char* fragString);


	void LoadTexture(const char* textureFile, uint& textureID);
	void LoadCubemap(uint& cubemapID, const char* posX, const char* negX, 
		const char* posY, const char* negY,
		const char* posZ, const char* negZ);

	uint AddRenderableInstance(
		uint geometryIndex,
		const glm::mat4& position,
		uint shaderIndex);
	RenderableInstanceInfo* AddRenderableInstance_p(
		uint geometryIndex,
		const glm::mat4& position,
		uint shaderIndex);

	void UpdateGeometry(uint index,
		void* vertices,
		uint numVerts,
		ushort* indices,
		uint numIndices, 
		RenderMode mode);
	void UpdateRenderableInstance(uint index,
		uint geometryIndex,
		const glm::mat4& position,
		uint shaderIndex);
	void UpdateShader(uint index,
		const char* vertexFile, 
		const char* fragmentFile);
#pragma endregion

protected:
	void initializeGL();
	void shutdown();

	void resizeEvent(QResizeEvent *e);
	void paintGL();
	void createShadowMap();
	void paintScene(const DebugCamera& cam, const glm::mat4 projection);
	void shadowRenderableInstance(RenderableInstanceInfo& renderable, const glm::mat4& mvp);
	void paintRenderableInstance(RenderableInstanceInfo& renderable, const glm::mat4& mvp);
	void paintRenderableInstance(RenderableInstanceInfo& renderable, const glm::mat4& m, const glm::mat4& v, const glm::mat4& p);

public:
	void keyPressEvent(QKeyEvent *e);
	void wheelEvent(QWheelEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);

	

signals:
	void rendererInitialized();
	void preDraw();

	void MousePressed(glm::vec2);
	void MouseMoved(glm::vec2);

public slots:
	void RefreshRenderer();
};

#endif // RENDERER_H