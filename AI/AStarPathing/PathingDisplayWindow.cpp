#include "PathingDisplayWindow.h"

#include <AStarPath.h>
#include <DebugShapes.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <Slider.h>
#include <Qt\qfiledialog.h>
#include <Qt\qmainwindow.h>
#include <Qt\qmenubar.h>
#include <Qt\qaction.h>
#include <Qt\qcheckbox.h>
#include <Qt\qpushbutton.h>
#include <Qt\qstring.h>
#include <Qt\qtimer.h>
#include <Qt\qtoolbar.h>
#include <Renderer.h>
#include <ShapeData.h>
#include <UpdateClock.h>
#include <ctime>

#include <iostream>

void PathingDisplayWindow::Initialize()
{
#pragma region RENDER SETUP
	renderer = new Renderer();
	renderer->VERTEX_SIZE = sizeof(Neumont::Vertex);
	renderer->INDEX_SIZE = sizeof(ushort);
	renderer->POSITION_OFFSET = Neumont::Vertex::POSITION_OFFSET;
	renderer->COLOR_OFFSET = Neumont::Vertex::COLOR_OFFSET;
	renderer->NORMAL_OFFSET = Neumont::Vertex::NORMAL_OFFSET;
	renderer->UV_OFFSET = Neumont::Vertex::UV_OFFSET;
	setCentralWidget(renderer);

	DebugShapes::SetRenderer(renderer);
#pragma endregion

	pathTreeRenderableInfo = 0;
	pathRenderableInfo = 0;

	updateTimer = new QTimer();
	connect(updateTimer, SIGNAL(timeout()), this, SLOT(Update()));
	updateTimer->start(16);

	loadLevelAction = menuBar()->addAction("Load Level");
	connect(loadLevelAction, SIGNAL(triggered()), this, SLOT(OpenLevel()));

	connect(renderer, SIGNAL(rendererInitialized()), this, SLOT(InitializeComponents()));

	QToolBar* tb = new QToolBar(QString::fromUtf8("Path Follower"), this);

	followerSpeed = 1.0f;
	followerSpeedSlider = new Neumont::Slider(0, 100, true);
	connect(followerSpeedSlider, SIGNAL(valueChanged(float)), this, SLOT(UpdateFollowerSpeed(float)));
	followerSpeedSlider->setValue(followerSpeed);	
	tb->addWidget(followerSpeedSlider);

	tb->addWidget(showAllNodes = new QCheckBox("Show all nodes"));
	showAllNodes->setChecked(true);
	connect(showAllNodes, SIGNAL(stateChanged(int)), this, SLOT(SetPathNodesVisibility(int)));

	tb->addWidget(showCurrentPath = new QCheckBox("Show current path"));
	showCurrentPath->setChecked(true);
	connect(showCurrentPath, SIGNAL(stateChanged(int)), this, SLOT(SetCurrentPathVisibility(int)));

	addToolBar(tb);



	std::srand(std::time(0));

	updateEnabled = false;

	UpdateClock::Initialize();
}

void PathingDisplayWindow::InitializeComponents()
{
	shaderID = renderer->AddShader("VertexShader.sh", "FragShader.sh");
}

void PathingDisplayWindow::Update()
{
	UpdateClock::Update();
	UpdateClock::SetDeltaTime(0.01666f);
	camera_position = renderer->camera.GetPosition();

	if(!updateEnabled) return;

	glm::mat4 trans = DebugShapes::GetShapeTransform(followerID);
	glm::vec3 pos = glm::vec3(trans[3][0], trans[3][1], trans[3][2]);
	glm::vec3 direction = level.nodes[path.nodeIndices[path.currentIndex]].position - pos;

#pragma region path stuff

	// check if we are close enough to the current node to start pathing towards the next node
	if(glm::length(direction) <= (0.05f * (0.2f * followerSpeed)))
	{
		path.currentIndex++;

		if(path.currentIndex >= path.indexCount)
		{
			path = AStarPath::FindPath(level.nodes, path.nodeIndices[path.currentIndex - 1], (std::rand() % level.numNodes));
			UpdatePathRenderableInfo();
			SetPathNodesVisibility(showAllNodes->checkState());
		}

		SetCurrentPathVisibility(showCurrentPath->checkState());
		return;
	}

#pragma endregion

#pragma region follower stuff

	glm::vec3 newPos = pos + (glm::normalize(direction) * UpdateClock::DeltaTime() * followerSpeed);

	glm::mat4 p = glm::translate(newPos);
	glm::mat4 r = glm::mat4();

	// rotation
	glm::vec3 original = glm::vec3(0,0,-1);
	glm::vec3 newOrientation = glm::normalize(direction);
	float yaw = acos(glm::dot(original, glm::normalize(glm::vec3(newOrientation.x, 0, newOrientation.z)))) * 180 / PI;
	float pitch = 0.0f;
	if(yaw * 0 != 0)
	{
		yaw = 0;
		pitch = acos(glm::dot(original, glm::normalize(newOrientation))) * 180 / PI;
	}
	else 
	{
		if(newOrientation.x < 0)
			yaw = -yaw;
		glm::vec3 pitchVector = glm::vec3(glm::vec4(original, 0) * glm::rotate(yaw, glm::vec3(0,1,0)));
		pitch = acos(glm::dot(pitchVector, glm::normalize(newOrientation))) * 180 / PI;
	}
	if(pitch * 0 != 0)
	{
		pitch = 0;
	}
	else if(newOrientation.y > 0)
	{
		pitch = -pitch;
	}

	r = glm::rotate(pitch, 1.0f,0.0f,0.0f) * glm::rotate(-yaw, 0.0f,1.0f,0.0f);

	DebugShapes::SetShapeTransform(followerID, 
		p * r);

#pragma endregion

	renderer->updateGL();
}

void PathingDisplayWindow::OpenLevel()
{
	QString filename = QFileDialog::getOpenFileName(this, "Open a level", "", "Native Levels (*.nlvl)");
	QByteArray ba = filename.toLocal8Bit();
	char* file = ba.data();
	if(filename == "" && (level.numRenderables == 0 && level.numNodes == 0)) { exit(0); }
	level = NativeLevel_Static::Load(file, renderer, shaderID);

	for(uint i = 0; i < level.numRenderables; i++)
	{
		level.renderables[i]->AddUniform("mvp", ut_MAT4_MVP, 0, 0);
		level.renderables[i]->AddUniform("modelToWorld", ut_MAT4, &level.renderables[i]->position[0][0], sizeof(glm::mat4));
		level.renderables[i]->AddUniform("modelToWorld_Normal", ut_MAT3, &glm::mat3(level.renderables[i]->position)[0][0], sizeof(glm::mat3));
		level.renderables[i]->AddUniform("ambient", ut_VEC3, &glm::vec3(0.5f, 0.5f, 0.5f)[0], sizeof(glm::vec3));
		level.renderables[i]->AddUniform("diffuse", ut_VEC3, &glm::vec3(0.5f,0.5f,0.5f)[0], sizeof(glm::vec3));
		level.renderables[i]->AddUniform("light_position", ut_VEC3, &glm::vec3(0,15,0)[0], sizeof(glm::vec3));
	}

	pathTreeRenderableInfo = new NodeRenderableInfo[level.numNodes];
	for(uint i = 0; i < level.numNodes; i++)
	{
		pathTreeRenderableInfo[i].nodeRenderableID = DebugShapes::MakeSphere(glm::translate(level.nodes[i].position) * glm::scale(0.15f,0.15f, 0.15f));

		pathTreeRenderableInfo[i].numConnections = level.nodes[i].numConnections;
		pathTreeRenderableInfo[i].connectionRenderableIDs = new uint[level.nodes[i].numConnections];
		for(uint c = 0; c < level.nodes[i].numConnections; c++)
		{
			uint id = DebugShapes::MakeVector(level.nodes[i].position, level.nodes[level.nodes[i].connections[c].connectorID].position);
			pathTreeRenderableInfo[i].connectionRenderableIDs[c] = id;
		}
	}

	glm::vec3 pos(0,0,0);
	if(level.numNodes >= 1)
		pos = level.nodes[0].position;

	followerID = DebugShapes::MakeArrow(glm::translate(pos) * glm::scale(0.45f, 0.45f, 0.45f), -1.0f, true, glm::vec3(0,0,1));

	menuBar()->removeAction(loadLevelAction);

	path = AStarPath::FindPath(level.nodes, 0, level.numNodes - 1);

	UpdatePathRenderableInfo();
	SetPathNodesVisibility(showAllNodes->checkState());
	SetCurrentPathVisibility(showCurrentPath->checkState());

	updateEnabled = true;
}

void PathingDisplayWindow::UpdatePathRenderableInfo()
{
	pathRenderableInfo = new NodeRenderableInfo[path.indexCount];

	for(uint i = 0; i < level.numNodes; i++)
	{
		DebugShapes::SetShapeColor(pathTreeRenderableInfo[i].nodeRenderableID, glm::vec3(1.0f, 0.0f, 0.0f));

		for(uint c = 0; c < level.nodes[i].numConnections; c++)
		{
			DebugShapes::SetShapeColor(pathTreeRenderableInfo[i].connectionRenderableIDs[c], glm::vec3(1.0f, 0.0f, 0.0f));
		}
	}

	for(uint i = 0; i < path.indexCount; i++)
	{
		pathRenderableInfo[i].nodeRenderableID = pathTreeRenderableInfo[path.nodeIndices[i]].nodeRenderableID;

		if(i < path.indexCount - 1)
		{
			pathRenderableInfo[i].numConnections = 1;
			pathRenderableInfo[i].connectionRenderableIDs = new uint[1];

			for(uint c = 0; c < level.nodes[path.nodeIndices[i]].numConnections; c++)
			{
				if(level.nodes[path.nodeIndices[i]].connections[c].connectorID == level.nodes[path.nodeIndices[i + 1]].ID)
				{
					pathRenderableInfo[i].connectionRenderableIDs[0] = pathTreeRenderableInfo[path.nodeIndices[i]].connectionRenderableIDs[c];
				}
			}
		}
		else
		{
			pathRenderableInfo[i].numConnections = 0;
		}		
	}
}

void PathingDisplayWindow::UpdateFollowerSpeed(float value)
{
	followerSpeed = value;
}
void PathingDisplayWindow::SetPathNodesVisibility(int state)
{
	bool visible = state == 2;	// checked
	showCurrentPath->setEnabled(!visible);
	if(visible) showCurrentPath->setCheckState(Qt::CheckState::Checked);

	if(pathTreeRenderableInfo == NULL) return;

	for(uint i = 0; i < level.numNodes; i++)
	{
		DebugShapes::SetShapeVisibility(pathTreeRenderableInfo[i].nodeRenderableID, visible);

		for(uint c = 0; c < pathTreeRenderableInfo[i].numConnections; c++)
		{
			DebugShapes::SetShapeVisibility(pathTreeRenderableInfo[i].connectionRenderableIDs[c], visible);
		}
	}

	SetCurrentPathVisibility(showCurrentPath->checkState());
}
void PathingDisplayWindow::SetCurrentPathVisibility(int state)
{
	if(pathRenderableInfo == NULL) return;

	bool visible = showCurrentPath->isChecked();
	for(uint i = 0; i < path.indexCount; i++)
	{
		DebugShapes::SetShapeVisibility(pathRenderableInfo[i].nodeRenderableID, visible);
		DebugShapes::SetShapeColor(pathRenderableInfo[i].nodeRenderableID, glm::vec3(0.0f, 1.0f, 0.0f));

		for(uint c = 0; c < pathRenderableInfo[i].numConnections; c++)
		{
			DebugShapes::SetShapeVisibility(pathRenderableInfo[i].connectionRenderableIDs[c], visible);
			DebugShapes::SetShapeColor(pathRenderableInfo[i].connectionRenderableIDs[c], glm::vec3(0.0f, 1.0f, 0.0f));
		}
		// TODO: set the connection to visible
	}
}

#include "PathingDisplayWindow_moc.cpp"