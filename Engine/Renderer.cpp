#include <cstring>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <Qt\qevent.h>
#include <Qt\qdebug.h>
#define ASSERTS
#include "Assertion.h"
#include "Renderer.h"

#include "ShadowShaders.inl"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#pragma region CONTROL SCHEME

#define FORWARD Qt::Key_W
#define BACKWARD Qt::Key_S
#define STRAFE_L Qt::Key_A
#define STRAFE_R Qt::Key_D
#define TURN_L Qt::Key_Q
#define TURN_R Qt::Key_E
#define TURN_U Qt::Key_T
#define TURN_D Qt::Key_G
#define UP Qt::Key_R
#define DOWN Qt::Key_F

#define MOUSE_TRACKING false

#pragma endregion

Renderer::Renderer(void)
{
	RendererInitialized = false;
	SplitView = false;
	CreateShadowMap = false;

	nextRenderableInstanceIndex = 0U;
	nextGeometryIndex = 0U;
	nextShaderIndex = 0U;

	activatedVertexBuffers = 0U;
	activatedIndexBuffers = 0U;

	for(uint i = 0; i < MAX_VERTEX_BUFFERS; i++)
	{
		currentVertexBufferOffset[i] = 0;
	}
	for(uint i = 0; i < MAX_INDEX_BUFFERS; i++)
	{
		currentIndexBufferOffset[i] = 0;
	}
}
Renderer::~Renderer(void)
{
	shutdown();
}
void Renderer::shutdown()
{
}

void Renderer::initializeGL()
{
	if(RendererInitialized) return;

	glewInit();

	setMinimumSize(600, 600);
	glClearColor(0,0,0,1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	//glEnable(GL_CULL_FACE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glCullFace(GL_BACK);	

	glGenBuffers(MAX_VERTEX_BUFFERS, vertexIDs);
	glGenBuffers(MAX_INDEX_BUFFERS, indexIDs);

#pragma region SHADOWS STUFF
	
	shadowProgramID = AddShader_CHARARRAY(shadow_vertex_shader_code, shadow_fragment_shader_code);

	// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-16-shadow-mapping/#Point_lights

	glGenFramebuffers(1, &shadowFrameBufferID);	
	glGenTextures(1, &shadowMapTexture);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFrameBufferID);
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width(), height(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMapTexture, 0);
	glDrawBuffer(GL_NONE);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) return;

#pragma endregion

	ActivateGLVertexBuffer();
	ActivateGLIndexBuffer();

	glViewport(0,0, width(), height());

	camera.SetPosition(0,0,-10);
	camera.SetTarget(0,0,10);

	camera_dbg.SetPosition(0,0,-10);
	camera_dbg.SetTarget(0,0,10);

	grabKeyboard();

	RendererInitialized = true;
	emit rendererInitialized();
}

uint Renderer::AddGeometry(void* vertices, uint numVerts, ushort* indices, uint numIndices, RenderMode mode)
{
	Assert(nextGeometryIndex < MAX_NUM_GEOMETRIES);
	GeometryInfo& current = geometries[nextGeometryIndex];

	current.renderMode = mode;
	current.vertexCount = numVerts;
	current.vertexBufferSize = numVerts * VERTEX_SIZE;

	AddGeometryToGLVertexBuffer(&current, vertices);

	current.indexCount = numIndices;
	current.indexBufferSize = numIndices * INDEX_SIZE;
	AddGeometryToGLIndexBuffer(&current, indices);

	qDebug() << "Geometry added (" << (nextGeometryIndex + 1) << " geometries total {index=" << nextGeometryIndex << "})";

	nextGeometryIndex++;
	return nextGeometryIndex - 1;
}
uint Renderer::AddShader(const char* vertexFile, const char* fragmentFile)
{
	Assert(nextShaderIndex < MAX_NUM_SHADERS);

	ShaderInfo& current = shaders[nextShaderIndex];
	ShaderManager::CreateProgram(&current.programID, &current.vertexShaderID, &current.fragmentShaderID, vertexFile, fragmentFile);
	
	qDebug() << "Shader added (" << (nextShaderIndex + 1) << " shaders total )";

	nextShaderIndex++;
	return nextShaderIndex - 1;
}

uint Renderer::AddShader_CHARARRAY(const char* vertexString, const char* fragString)
{
	Assert(nextShaderIndex < MAX_NUM_SHADERS);

	ShaderInfo& current = shaders[nextShaderIndex];
	current.programID = ShaderManager::CreateProgram_CHARARRAY(vertexString, fragString);

	qDebug() << "Shader added [from strings] (" << (nextShaderIndex + 1) << " shaders total )";

	nextShaderIndex++;
	return nextShaderIndex - 1;
}
uint Renderer::AddRenderableInstance(uint geometryIndex, const glm::mat4& position, uint shaderIndex)
{
	Assert(nextRenderableInstanceIndex < MAX_NUM_RENDERABLES);

	RenderableInstanceInfo& current = renderableInstances[nextRenderableInstanceIndex];
	current.position = position;
	current.geometry = &geometries[geometryIndex];
	current.shader = &shaders[shaderIndex];
	current.uniformsCount = 0;

	current.visible = true;

	qDebug() << "Renderable added (" << (nextRenderableInstanceIndex + 1) << " renderables total )";

	paintGL();

	nextRenderableInstanceIndex++;	
	return nextRenderableInstanceIndex - 1;
}
Renderer::RenderableInstanceInfo* Renderer::AddRenderableInstance_p(uint geometryIndex, const glm::mat4& position, uint shaderIndex)
{
	Assert(nextRenderableInstanceIndex < MAX_NUM_RENDERABLES);

	RenderableInstanceInfo& current = renderableInstances[nextRenderableInstanceIndex];
	current.position = position;
	current.geometry = &geometries[geometryIndex];
	current.shader = &shaders[shaderIndex];
	current.uniformsCount = 0;
	
	current.visible = true;

	qDebug() << "Renderable added (pointer) (" << (nextRenderableInstanceIndex + 1) << " renderables total )";
	
	paintGL();

	nextRenderableInstanceIndex++;
	return &renderableInstances[nextRenderableInstanceIndex - 1];
}
void Renderer::LoadTexture(const char* textureFile, uint& textureID)
{
	//glActiveTexture(GL_TEXTURE4);

	//glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0 + textureID);

	QImage textureImage = QGLWidget::convertToGLFormat(QImage(textureFile, "PNG"));	// try removing "PNG" and see if it still works
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureImage.width(), textureImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, textureImage.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}
void Renderer::LoadCubemap(uint& cubemapID, const char* posX, const char* negX, const char* posY, const char* negY, const char* posZ, const char* negZ)
{
//	glActiveTexture(GL_TEXTURE2);
	
	glGenTextures(1, &cubemapID);

	glActiveTexture(GL_TEXTURE0 + cubemapID);

	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);

	QImage loadedImg = QGLWidget::convertToGLFormat(QImage(posX, "PNG"));
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, loadedImg.width(), loadedImg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, loadedImg.bits());	
	loadedImg = QGLWidget::convertToGLFormat(QImage(negX, "PNG"));
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, loadedImg.width(), loadedImg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, loadedImg.bits());
	loadedImg = QGLWidget::convertToGLFormat(QImage(posY, "PNG"));
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, loadedImg.width(), loadedImg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, loadedImg.bits());
	loadedImg = QGLWidget::convertToGLFormat(QImage(negY, "PNG"));
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, loadedImg.width(), loadedImg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, loadedImg.bits());
	loadedImg = QGLWidget::convertToGLFormat(QImage(posZ, "PNG"));
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, loadedImg.width(), loadedImg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, loadedImg.bits());
	loadedImg = QGLWidget::convertToGLFormat(QImage(negZ, "PNG"));
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, loadedImg.width(), loadedImg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, loadedImg.bits());

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Renderer::UpdateGeometry(uint index, void* vertices, uint numVerts, ushort* indices, uint numIndices, RenderMode mode)
{
	Assert(index < MAX_NUM_GEOMETRIES);

	GeometryInfo& current = geometries[index];
	Assert(numVerts <= current.vertexBufferSize);	// for now-- don't allow updates that would increase the original size of the buffer.
	Assert(numVerts != NULL);
	Assert(vertices != NULL);
	Assert(numIndices <= current.indexBufferSize);	// for now-- don't allow updates that would increase the original size of the buffer.
	Assert(numIndices != NULL);
	Assert(indices != NULL);
	
	current.renderMode = mode;

	current.vertexCount = numVerts;
	current.vertexBufferSize = numVerts * VERTEX_SIZE;
	UpdateGeometryInGLVertexBuffer(&current, vertices);
	
	current.indexCount = numIndices;
	current.indexBufferSize = numIndices * INDEX_SIZE;
	UpdateGeometryInGLIndexBuffer(&current, indices);

}
void Renderer::UpdateShader(uint index, const char* vertexFile, const char* fragmentFile)
{
	Assert(index < MAX_NUM_SHADERS);

	ShaderInfo& current = shaders[index];
	current.programID = ShaderManager::CreateProgram(vertexFile, fragmentFile);
}
void Renderer::UpdateRenderableInstance(uint index, uint geometryID, const glm::mat4& position, uint shaderProgramID)
{
	Assert(index < MAX_NUM_RENDERABLES);
	Assert(geometryID < MAX_NUM_GEOMETRIES);
	Assert(shaderProgramID < MAX_NUM_SHADERS);

	RenderableInstanceInfo& current = renderableInstances[index];
	current.position = position;
	current.geometry = &geometries[geometryID];
	current.shader = &shaders[shaderProgramID];
	current.visible = true;

}

void Renderer::AddGeometryToGLVertexBuffer(GeometryInfo* geometry, void* vertices)
{
	// loop over every activated buffer, find one that has enough room to store the geometry.
	for(uint i = 0; i < activatedVertexBuffers; i++)
	{
		if(geometry->vertexBufferSize < (BUFFER_SIZE - currentVertexBufferOffset[i]))
		{
			// enough room in this vertex buffer; store it here!
			glBindBuffer(GL_ARRAY_BUFFER, vertexIDs[i]);

			geometry->vertexBufferID = i;
			geometry->vertexBufferOffset = currentVertexBufferOffset[i];

			glBufferSubData(GL_ARRAY_BUFFER, 
				geometry->vertexBufferOffset,
				geometry->vertexBufferSize,
				vertices);

			currentVertexBufferOffset[i] += geometry->vertexBufferSize;

			return;
		}
	}

	// if there was no room in any activated buffers, and we haven't activated the max
	// number of buffers, activate a new one then recall this function.
	if(activatedVertexBuffers < MAX_VERTEX_BUFFERS)
	{
		ActivateGLVertexBuffer();
		AddGeometryToGLVertexBuffer(geometry, vertices);
	}
}
void Renderer::AddGeometryToGLIndexBuffer(GeometryInfo* geometry, ushort* indices)
{
	// get next available index buffer
	for(uint i = 0; i < activatedIndexBuffers; i++)
	{
		if(geometry->indexBufferSize < (BUFFER_SIZE - currentIndexBufferOffset[i]))
		{
			// enough room in this vertex buffer; store it here!
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexIDs[i]);

			geometry->indexBufferID = i;
			geometry->indexBufferOffset = currentIndexBufferOffset[i];

			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
				geometry->indexBufferOffset,
				geometry->indexBufferSize,
				indices);

			currentIndexBufferOffset[i] += geometry->indexBufferSize;

			return;
		}
	}

	// if there was no room in any activated buffers, and we haven't activated the max
	// number of buffers, activate a new one then recall this function.
	if(activatedIndexBuffers < MAX_INDEX_BUFFERS)
	{
		ActivateGLIndexBuffer();
		AddGeometryToGLIndexBuffer(geometry, indices);
	}
}
void Renderer::UpdateGeometryInGLVertexBuffer(GeometryInfo* geometry, void* vertices)
{
	glBindBuffer(GL_ARRAY_BUFFER, vertexIDs[geometry->vertexBufferID]);

	glBufferSubData(GL_ARRAY_BUFFER,
		geometry->vertexBufferOffset,
		geometry->vertexBufferSize,
		vertices);
}
void Renderer::UpdateGeometryInGLIndexBuffer(GeometryInfo* geometry, ushort* indices)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexIDs[geometry->indexBufferID]);

	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
		geometry->indexBufferOffset,
		geometry->indexBufferSize,
		indices);
}

void Renderer::ActivateGLVertexBuffer()
{
	uint index = activatedVertexBuffers;
	
	// activate a new buffer.
	glBindBuffer(GL_ARRAY_BUFFER, vertexIDs[index]);
	glBufferData(GL_ARRAY_BUFFER, BUFFER_SIZE, NULL, GL_STATIC_DRAW);
	
	activatedVertexBuffers++;
}
void Renderer::ActivateGLIndexBuffer()
{
	uint index = activatedIndexBuffers;

	// activate a new buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexIDs[index]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, BUFFER_SIZE, NULL, GL_STATIC_DRAW);

	activatedIndexBuffers++;
}
void Renderer::keyPressEvent(QKeyEvent *e)
{
	if(SplitView)
	{
		// FORWARD
		if(e->key() == FORWARD)
			camera_dbg.MoveForward(0.25f);
		if(e->key() == BACKWARD)
			camera_dbg.MoveForward(-0.25f);
	
		// ROTATE-HORIZONTAL
		if(e->key() == TURN_L)
			camera_dbg.RotateHorizontal(5.0f);
		if(e->key() == TURN_R)
			camera_dbg.RotateHorizontal(-5.0f);

		// STRAFE
		if(e->key() == STRAFE_L)
			camera_dbg.MoveSideways(0.25f);
		if(e->key() == STRAFE_R)
			camera_dbg.MoveSideways(-0.25f);

		// UP DOWN
		if(e->key() == UP)
			camera_dbg.MoveVertical(0.25f);
		if(e->key() == DOWN)
			camera_dbg.MoveVertical(-0.25f);

		// ROTATE-VERTICAL
		if(e->key() == TURN_U)
			camera_dbg.RotateVertical(5.0f);
		if(e->key() == TURN_D)
			camera_dbg.RotateVertical(-5.0f);
	}
	else
	{
		// FORWARD
		if(e->key() == FORWARD)
			camera.MoveForward(0.25f);
		if(e->key() == BACKWARD)
			camera.MoveForward(-0.25f);
	
		// ROTATE-HORIZONTAL
		if(e->key() == TURN_L)
			camera.RotateHorizontal(5.0f);
		if(e->key() == TURN_R)
			camera.RotateHorizontal(-5.0f);

		// STRAFE
		if(e->key() == STRAFE_L)
			camera.MoveSideways(0.25f);
		if(e->key() == STRAFE_R)
			camera.MoveSideways(-0.25f);

		// UP DOWN
		if(e->key() == UP)
			camera.MoveVertical(0.25f);
		if(e->key() == DOWN)
			camera.MoveVertical(-0.25f);

		// ROTATE-VERTICAL
		if(e->key() == TURN_U)
			camera.RotateVertical(5.0f);
		if(e->key() == TURN_D)
			camera.RotateVertical(-5.0f);


		camera_dbg.SetPosition(camera.GetPosition());
		camera_dbg.SetTarget(camera.direction);
	}

	glDraw();
}
void Renderer::wheelEvent(QWheelEvent *e)
{
	if(SplitView)
	{
		camera_dbg.MoveForward((float)(e->delta() * 0.005f));
	}
	else
	{
		camera.MoveForward((float)(e->delta() * 0.005f));

		camera_dbg.SetPosition(camera.GetPosition());
		camera_dbg.SetTarget(camera.direction);
	}

	glDraw();
}
void Renderer::mousePressEvent(QMouseEvent *e)
{
	lastMousePos = glm::vec2(e->x(), e->y());

	emit MousePressed(glm::vec2(e->x(), e->y()));

	glDraw();
}
void Renderer::mouseMoveEvent(QMouseEvent *e)
{
	glm::vec2 newPos = glm::vec2(e->x(), e->y());

	if(glm::distance(newPos, lastMousePos) < 100)
	{
		if(SplitView)
		{
			camera_dbg.RotateHorizontal((lastMousePos.x - newPos.x) * 0.4f);
			camera_dbg.RotateVertical((lastMousePos.y - newPos.y) * 0.4f);
		}
		else
		{
			camera.RotateHorizontal((lastMousePos.x - newPos.x) * 0.4f);
			camera.RotateVertical((lastMousePos.y - newPos.y) * 0.4f);

			camera_dbg.SetPosition(camera.GetPosition());
			camera_dbg.SetTarget(camera.direction);
		}
		lastMousePos = newPos;
	}

	emit MouseMoved(glm::vec2(e->x(), e->y()));

	glDraw();
}

void Renderer::resizeEvent(QResizeEvent *e)
{
	glViewport(0,0, e->size().width(), e->size().height());

	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width(), height(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	qDebug() << "new render window size: " << e->size().width() << ", " << e->size().height();
}
void Renderer::RefreshRenderer()
{
	qDebug() << "RefreshRenderer() called";
	repaint();
}

void Renderer::paintGL()
{
	emit preDraw();

	glViewport(0,0, (float)width(), (float)height());
	glScissor(0,0, (float)width(), (float)height());

	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	glClearColor(0,0,0,1);

	if(SplitView)
	{
		projection = glm::perspective(
			60.0f,
			(width() / 2.0f) / height(),
			0.1f, 100.0f);
		
		glViewport(0,0, (width() / 2.0f) - 5, height());
		glScissor(0,0, (width() / 2.0f) - 5, height());
		glClear(GL_COLOR_BUFFER_BIT);
		paintScene(camera, projection);

		glViewport((width() / 2.0f) + 5, 0, (width() / 2.0f) - 5, height());
		glScissor((width() / 2.0f) + 5, 0, (width() / 2.0f) - 5, height());
		glClear(GL_COLOR_BUFFER_BIT);
		paintScene(camera_dbg, projection);
	}
	else
	{
		projection = glm::perspective(
			60.0f,
			(float)width() / height(),
			0.1f, 500.0f);
		glm::mat4 view = camera.GetView();

		glClear(GL_COLOR_BUFFER_BIT);
		if(CreateShadowMap)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, shadowFrameBufferID);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			createShadowMap();
		}
		paintScene(camera, projection);

		// CODE TO PAINT THE SHADOWMAP PREVIEW
		//	if(CreateShadowMap)
		//	{
		//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//		glViewport(0, -20, width()/6.0f, height()/4.0f);
		//		createShadowMap();
		//	}
	}
}

void Renderer::createShadowMap()
{
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10,10,-10,10,-10,20);
	glm::mat4 projectedView = depthProjectionMatrix * glm::lookAt(lightPosition, lightPosition + lightDirection, glm::vec3(0,1,0));

	for(uint i = 0; i < nextRenderableInstanceIndex; i++)
	{
		if(renderableInstances[i].layer == rl_TOPMOST)// || renderableInstances[i].layer == rl_TRANSPARENT)
			continue;

		glm::mat4 mvp = projectedView * renderableInstances[i].position;
		shadowRenderableInstance(renderableInstances[i], mvp);
	}
}
void Renderer::paintScene(const DebugCamera& cam, const glm::mat4 projection)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projectedView = projection * cam.GetView();

	// Render based on layers:
	// steps: http://stackoverflow.com/a/3390094/3233950
	// 1. Enable writing to the depth buffer ( glDepthMask(GL_TRUE); )
	// 2. Render all opaque objects, order doesn't matter.
	// 3. Disable the depth mask (stop writing to the depth buffer) ( glDepthMask(GL_FALSE); )
	// 4. Enable a blending mode ( glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); )
	// 5. Render translucent objects sorted from farthest->nearest
	
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);	// enable depth test for opaque objects
	glDisable(GL_BLEND);
	for(uint i = 0; i < nextRenderableInstanceIndex; i++)	// render opaque layer first
	{
		if(renderableInstances[i].layer != rl_OPAQUE)
			continue;

		glm::mat4 mvp = projectedView * renderableInstances[i].position;
		//paintRenderableInstance(renderableInstances[i], mvp);
		paintRenderableInstance(renderableInstances[i], renderableInstances[i].position, cam.GetView(), projection);
	}

	//glDisable(GL_DEPTH_TEST);	// disable depth test for non-opaque objects
	glEnable(GL_BLEND);
	for(uint i = 0; i < nextRenderableInstanceIndex; i++)	// render opaque layer first
	{
		if(renderableInstances[i].layer != rl_TRANSPARENT)	// render transparent layer second
			continue;

		// TODO: Don't draw here-- need to sort the transparent objects based on distance so that we draw the closest ones last
		glm::mat4 mvp = projectedView * renderableInstances[i].position;
		//paintRenderableInstance(renderableInstances[i], mvp);
		paintRenderableInstance(renderableInstances[i], renderableInstances[i].position, cam.GetView(), projection);
	}
	glDisable(GL_BLEND);
	for(uint i = 0; i < nextRenderableInstanceIndex; i++)	// render top-most layer last
	{
		if(renderableInstances[i].layer != rl_TOPMOST)
			continue;

		glm::mat4 mvp = projectedView * renderableInstances[i].position;
		//paintRenderableInstance(renderableInstances[i], mvp);
		paintRenderableInstance(renderableInstances[i], renderableInstances[i].position, cam.GetView(), projection);
	}
}
void Renderer::shadowRenderableInstance(RenderableInstanceInfo& renderable, const glm::mat4& mvp)
{
	if(!renderable.visible)
		return;

	glUseProgram(shaders[shadowProgramID].programID);

	uint loc = glGetUniformLocation(shaders[shadowProgramID].programID, "mvp");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &mvp[0][0]);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(POSITION_OFFSET + renderable.geometry->vertexBufferOffset));

	switch(renderable.geometry->renderMode)
	{
	case rm_LINES:
		glDrawElements(GL_LINES, renderable.geometry->indexCount, GL_UNSIGNED_SHORT, BUFFER_OFFSET(renderable.geometry->indexBufferOffset));
	case rm_TRIANGLES:
		glDrawElements(GL_TRIANGLES, renderable.geometry->indexCount, GL_UNSIGNED_SHORT, BUFFER_OFFSET(renderable.geometry->indexBufferOffset));
	}
}
void Renderer::paintRenderableInstance(RenderableInstanceInfo& renderable, const glm::mat4& mvp)
{
	if(!renderable.visible)	// TODO: make an invisible layer and remove the need for this completely.
		return;

	uint program_ID = renderable.shader->programID;
	glUseProgram(program_ID);




	// TODO: REMOVE THIS GROSS HARDCODED SHADOW UNIFORM STUFF!!!!!
	uint shadowMapLoc = glGetUniformLocation(program_ID, "shadowMap");
	uint depthMVPLoc = glGetUniformLocation(program_ID, "shadowmapMVP");

	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0, 
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10,10,-10,10,-10,20);
	glm::mat4 depthMVP = biasMatrix * depthProjectionMatrix * glm::lookAt(lightPosition, lightPosition + lightDirection, glm::vec3(0,1,0)) * renderable.position;
	glUniformMatrix4fv(depthMVPLoc, 1, GL_FALSE, &depthMVP[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	glUniform1i(shadowMapLoc, 0);

	uint normalMapLoc = glGetUniformLocation(program_ID, "normalMap");
	if(normalMapLoc != (uint)-1)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normalMapTexture);
		glUniform1i(normalMapLoc, 1);
	}

	uint uniformLoc;
	for(uint i = 0; i < renderable.uniformsCount; i++)
	{
		uniformLoc = glGetUniformLocation(program_ID, renderable.uniforms[i].name);

		switch(renderable.uniforms[i].type)
		{
		case ut_FLOAT:
			glUniform1fv(uniformLoc, 1, reinterpret_cast<float*>(renderable.uniforms[i].data));
			break;
		case ut_INT:
			glUniform1iv(uniformLoc, 1, reinterpret_cast<int*>(renderable.uniforms[i].data));
			break;
		case ut_VEC2:
			glUniform2fv(uniformLoc, 1, reinterpret_cast<float*>(renderable.uniforms[i].data));
			break;
		case ut_VEC3:
			glUniform3fv(uniformLoc, 1, reinterpret_cast<float*>(renderable.uniforms[i].data));
			break;
		case ut_VEC4:
			glUniform4fv(uniformLoc, 1, reinterpret_cast<float*>(renderable.uniforms[i].data));
			break;
		case ut_MAT3:
			glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, reinterpret_cast<float*>(renderable.uniforms[i].data));
			break;
		case ut_MAT4:
			glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, reinterpret_cast<float*>(renderable.uniforms[i].data));
			break;
		case ut_SAMPLER:
		{
			glUniform1i(uniformLoc, *reinterpret_cast<int*>(renderable.uniforms[i].data));
			break;
		}
		case ut_MAT4_MVP:
		{
			glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, &mvp[0][0]);
			break;
		}
		case ut_NULL:
		default:
			break;
		}
	}


	// -----------------------------------------------------------

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(POSITION_OFFSET + renderable.geometry->vertexBufferOffset));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(COLOR_OFFSET + renderable.geometry->vertexBufferOffset));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(NORMAL_OFFSET + renderable.geometry->vertexBufferOffset));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(UV_OFFSET + renderable.geometry->vertexBufferOffset));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(NORMAL_TANGENT_OFFSET + renderable.geometry->vertexBufferOffset));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(NORMAL_BITANGENT_OFFSET + renderable.geometry->vertexBufferOffset));

	switch(renderable.geometry->renderMode)
	{
	case rm_LINES:
		glDrawElements(GL_LINES, renderable.geometry->indexCount, GL_UNSIGNED_SHORT, BUFFER_OFFSET(renderable.geometry->indexBufferOffset));
	case rm_TRIANGLES:
		glDrawElements(GL_TRIANGLES, renderable.geometry->indexCount, GL_UNSIGNED_SHORT, BUFFER_OFFSET(renderable.geometry->indexBufferOffset));
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);
}
void Renderer::paintRenderableInstance(RenderableInstanceInfo& renderable, const glm::mat4& m, const glm::mat4& v, const glm::mat4& p)
{
	if(!renderable.visible)	// TODO: make an invisible layer and remove the need for this completely.
		return;

	uint program_ID = renderable.shader->programID;
	glUseProgram(program_ID);




	// TODO: REMOVE THIS GROSS HARDCODED SHADOW UNIFORM STUFF!!!!!
	uint shadowMapLoc = glGetUniformLocation(program_ID, "shadowMap");
	uint depthMVPLoc = glGetUniformLocation(program_ID, "shadowmapMVP");

	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0, 
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10,10,-10,10,-10,20);
	glm::mat4 depthMVP = biasMatrix * depthProjectionMatrix * glm::lookAt(lightPosition, lightPosition + lightDirection, glm::vec3(0,1,0)) * renderable.position;
	glUniformMatrix4fv(depthMVPLoc, 1, GL_FALSE, &depthMVP[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	glUniform1i(shadowMapLoc, 0);

	uint normalMapLoc = glGetUniformLocation(program_ID, "normalMap");
	if(normalMapLoc != (uint)-1)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normalMapTexture);
		glUniform1i(normalMapLoc, 1);
	}

	uint uniformLoc;
	for(uint i = 0; i < renderable.uniformsCount; i++)
	{
		uniformLoc = glGetUniformLocation(program_ID, renderable.uniforms[i].name);

 		switch(renderable.uniforms[i].type)
		{
		case ut_FLOAT:
			glUniform1fv(uniformLoc, 1, reinterpret_cast<float*>(renderable.uniforms[i].data));
			break;
		case ut_INT:
			glUniform1iv(uniformLoc, 1, reinterpret_cast<int*>(renderable.uniforms[i].data));
			break;
		case ut_VEC2:
			glUniform2fv(uniformLoc, 1, reinterpret_cast<float*>(renderable.uniforms[i].data));
			break;
		case ut_VEC3:
			glUniform3fv(uniformLoc, 1, reinterpret_cast<float*>(renderable.uniforms[i].data));
			break;
		case ut_VEC4:
			glUniform4fv(uniformLoc, 1, reinterpret_cast<float*>(renderable.uniforms[i].data));
			break;
		case ut_MAT3:
			glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, reinterpret_cast<float*>(renderable.uniforms[i].data));
			break;
		case ut_MAT4:
			glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, reinterpret_cast<float*>(renderable.uniforms[i].data));
			break;
		case ut_CUBE_MAP:
		{
			glUniform1i(uniformLoc, *reinterpret_cast<int*>(renderable.uniforms[i].data));
		}
		case ut_TEXTURE:
		case ut_SAMPLER:
		{
			glUniform1i(uniformLoc, *reinterpret_cast<int*>(renderable.uniforms[i].data));
			break;
		}
		case ut_MAT4_MVP:
		{
			glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, &(p * v * m)[0][0]);
			break;
		}
		case ut_MAT4_Model:
		{
			glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, &m[0][0]);
			break;
		}
		case ut_MAT4_View:
		{
			glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, &v[0][0]);
			break;
		}
		case ut_MAT4_Projection:
		{
			glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, &p[0][0]);
			break;
		}
		case ut_NULL:
		default:
			break;
		}
	}


	// -----------------------------------------------------------

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(POSITION_OFFSET + renderable.geometry->vertexBufferOffset));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(COLOR_OFFSET + renderable.geometry->vertexBufferOffset));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(NORMAL_OFFSET + renderable.geometry->vertexBufferOffset));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(UV_OFFSET + renderable.geometry->vertexBufferOffset));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(NORMAL_TANGENT_OFFSET + renderable.geometry->vertexBufferOffset));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(NORMAL_BITANGENT_OFFSET + renderable.geometry->vertexBufferOffset));

	switch(renderable.geometry->renderMode)
	{
	case rm_LINES:
		glDrawElements(GL_LINES, renderable.geometry->indexCount, GL_UNSIGNED_SHORT, BUFFER_OFFSET(renderable.geometry->indexBufferOffset));
	case rm_TRIANGLES:
		glDrawElements(GL_TRIANGLES, renderable.geometry->indexCount, GL_UNSIGNED_SHORT, BUFFER_OFFSET(renderable.geometry->indexBufferOffset));
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);
}


void Renderer::ShaderUniform::Initialize(const char* n, UniformType t, void* d, uint dataSize)
{
	memset(name, '\0', MAX_UNIFORM_DATA_SIZE);
	memcpy(name, n, strlen(n));

	memcpy(data, d, dataSize);

	type = t;
}
uint Renderer::RenderableInstanceInfo::AddUniform(const char* n, UniformType t, void* d, uint dataSize)
{
	Assert(uniformsCount < MAX_UNIFORMS_PER_SHADER);

	uniforms[uniformsCount].Initialize(n, t, d, dataSize);
	uniformsCount++;
	return uniformsCount - 1;
}
void Renderer::RenderableInstanceInfo::UpdateUniform(uint index, void* d, uint dataSize)
{
	Assert(index < uniformsCount);

	memcpy(uniforms[index].data, d, dataSize);
}

void Renderer::RayFromMouseCoords(glm::vec2 p, glm::vec3* rayorigin, glm::vec3* raydirection)
{
	glm::vec3 worldPos;

	// Unproject the point
	worldPos.x = (((2.0f * p.x) / width()) - 1) / projection[0][0];
	worldPos.y = -(((2.0f * p.y) / height()) - 1) / projection[1][1];
	worldPos.z = -1.0f;

	// Remove the view matrix (using its inverse)
	glm::mat4 invView = camera_dbg.GetInverseView();

	raydirection->x = (worldPos.x * invView[0][0]) + (worldPos.y * invView[1][0]) + (worldPos.z * invView[2][0]);
	raydirection->y = (worldPos.x * invView[0][1]) + (worldPos.y * invView[1][1]) + (worldPos.z * invView[2][1]);
	raydirection->z = (worldPos.x * invView[0][2]) + (worldPos.y * invView[1][2]) + (worldPos.z * invView[2][2]);
	rayorigin->x = invView[3][0];
	rayorigin->y = invView[3][1];
	rayorigin->z = invView[3][2];

	*raydirection = glm::normalize(*raydirection);
}