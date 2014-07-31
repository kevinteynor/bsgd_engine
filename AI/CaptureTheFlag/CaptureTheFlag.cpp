#include "CaptureTheFlag.h"

#include <AStarPath.h>
#include <ctime>
#include <DebugShapes.h>
#include <EngineTypeDefs.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <NativeLevel.h>
#include <Qt\qfiledialog.h>
#include <Qt\qmainwindow.h>
#include <Qt\qmenubar.h>
#include <Qt\qtimer.h>
#include <Renderer.h>
#include <ShapeData.h>
#include <UpdateClock.h>

CaptureTheFlag::CaptureTheFlag()
{
	players = new Player*[NUM_PLAYERS];
	TeamFlags[0].carrier = 0;
	TeamFlags[1].carrier = 0;
	Initialize();
}

void CaptureTheFlag::Initialize()
{
#pragma region RENDERER
	renderer = new Renderer();
	renderer->VERTEX_SIZE = sizeof(Neumont::Vertex);
	renderer->INDEX_SIZE = sizeof(ushort);
	renderer->POSITION_OFFSET = Neumont::Vertex::POSITION_OFFSET;
	renderer->COLOR_OFFSET = Neumont::Vertex::COLOR_OFFSET;
	renderer->NORMAL_OFFSET = Neumont::Vertex::NORMAL_OFFSET;
	renderer->UV_OFFSET = Neumont::Vertex::UV_OFFSET;
	setCentralWidget(renderer);

	connect(renderer, SIGNAL(rendererInitialized()), this, SLOT(OpenLevel()));

	DebugShapes::SetRenderer(renderer);
#pragma endregion

	std::srand(std::time(0));
	UpdateClock::Initialize();

	updateTimer = new QTimer();
	updateTimer->start(16);
	connect(updateTimer, SIGNAL(timeout()), this, SLOT(Update()));

	levelLoaded = false;
}

void CaptureTheFlag::Update()
{
	UpdateClock::Update();
	UpdateClock::SetDeltaTime(0.016666f);

	if(!levelLoaded) return;

	for(uint i = 0; i < NUM_PLAYERS; i++)
	{
		players[i]->Update();	
	}

	if(TeamFlags[0].carrier != 0)
	{
		TeamFlags[0].position = TeamFlags[0].carrier->position;
		DebugShapes::SetShapeTransform(TeamFlags[0].renderableID, glm::translate(TeamFlags[0].position) * glm::scale(0.35f, 0.35f, 0.35f));
	}
	if(TeamFlags[1].carrier != 0)
	{
		TeamFlags[1].position = TeamFlags[1].carrier->position;
		DebugShapes::SetShapeTransform(TeamFlags[1].renderableID, glm::translate(TeamFlags[1].position) * glm::scale(0.35f, 0.35f, 0.35f));
	}

	renderer->updateGL();
}

void CaptureTheFlag::CaptureFlag(Player* p)
{
	ResetFlag(p->teamID == 0 ? 1 : 0);

	// add score
}
void CaptureTheFlag::ResetFlag(uint teamID)
{
	TeamFlags[teamID].position = level.nodes[teamID == 0 ? 32 : 33].position;
	TeamFlags[teamID].carrier = 0;

	DebugShapes::SetShapeTransform(TeamFlags[teamID].renderableID, glm::translate(TeamFlags[teamID].position) * glm::scale(0.35f, 0.35f, 0.35f));
}

void CaptureTheFlag::OpenLevel()
{
	uint shaderID = renderer->AddShader("VertexShader.sh", "FragShader.sh");

	QString filename = QFileDialog::getOpenFileName(this, "Open a level", "", "Native Levels (*.nlvl)");
	if(filename == "") { exit(0); }
	QByteArray ba = filename.toLocal8Bit();
	char* file = ba.data();
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

	team1_HomeNodeID = level.nodes[32].ID;
	team2_HomeNodeID = level.nodes[33].ID;

	TeamFlags[0].position = level.nodes[32].position;
	TeamFlags[0].renderableID = DebugShapes::MakeCube(glm::translate(TeamFlags[0].position) * glm::scale(0.35f, 0.35f, 0.35f), -1.0f, true, glm::vec3(1,0,0));
	TeamFlags[0].homeNodeID = 32;
	TeamFlags[1].position = level.nodes[33].position;
	TeamFlags[1].renderableID = DebugShapes::MakeCube(glm::translate(TeamFlags[1].position) * glm::scale(0.35f, 0.35f, 0.35f), -1.0f, true, glm::vec3(0,0,1));
	TeamFlags[1].homeNodeID = 33;

	players[0] = new Player(this, 0, 32, 16, glm::vec3(1,0,0));
	players[1] = new Player(this, 0, 32, 16, glm::vec3(1,0,0));
	players[2] = new Player(this, 1, 33, 16, glm::vec3(0,0,1));
	players[3] = new Player(this, 1, 33, 16, glm::vec3(0,0,1));

	players[0]->endTargetNodeID = 33;
	players[1]->endTargetNodeID = 33;
	players[2]->endTargetNodeID = 32;
	players[3]->endTargetNodeID = 32;

	players[0]->defensive = std::rand() % 2;
	players[1]->defensive = std::rand() % 2;
	players[2]->defensive = std::rand() & 2;
	players[3]->defensive = std::rand() % 2;

	if(players[0]->defensive)
		DebugShapes::SetShapeColor(players[0]->shapeID, glm::vec3(1.0f,0.25f,0.0f));
	if(players[1]->defensive)
		DebugShapes::SetShapeColor(players[1]->shapeID, glm::vec3(1.0f,0.25f,0.0f));
	if(players[2]->defensive)
		DebugShapes::SetShapeColor(players[2]->shapeID, glm::vec3(0.0f,0.25f,1.0f));
	if(players[3]->defensive)
		DebugShapes::SetShapeColor(players[3]->shapeID, glm::vec3(0.0f,0.25f,1.0f));


	levelLoaded = true;
}

#include "CaptureTheFlag_moc.cpp"