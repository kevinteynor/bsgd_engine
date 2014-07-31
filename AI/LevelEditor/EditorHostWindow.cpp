#include "EditorHostWindow.h"

#include <AStarPath.h>
#include <DebugCamera.h>
#include <DebugShapes.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <NativeModel.h>
#include <Qt\qaction.h>
#include <Qt\qboxlayout.h>
#include <Qt\qcheckbox.h>
#include <Qt\qevent.h>
#include <Qt\qfiledialog.h>
#include <Qt\qlabel.h>
#include <Qt\qlineedit.h>
#include <Qt\qmenu.h>
#include <Qt\qmenubar.h>
#include <Qt\qsizepolicy.h>
#include <Qt\qstatusbar.h>
#include <Qt\qtoolbar.h>
#include <Qt\qvalidator.h>
#include <Renderer.h>
#include <ShapeData.h>
#include <Slider.h>

#include <iostream>

void EditorHostWindow::Initialize()
{
	instanceInfo = 0;

	menuBar()->addMenu(BuildMenu_File());
	menuBar()->addMenu(BuildMenu_Help());
	addToolBar(BuildToolBar_AStarPathing());
	statusBar();

	cameraPositionLabel = new QLabel();
	statusBar()->addPermanentWidget(cameraPositionLabel);

	InitializeRenderer();
	connect(renderer, SIGNAL(rendererInitialized()), this, SLOT(InitializeComponents()));
	connect(renderer, SIGNAL(MousePressed(glm::vec2)), this, SLOT(Renderer_MousePressed(glm::vec2)));
	connect(renderer, SIGNAL(MouseMoved(glm::vec2)), this, SLOT(Renderer_MouseMoved(glm::vec2)));
	connect(renderer, SIGNAL(preDraw()), this, SLOT(Update()));
	selectedNodeIndex = (uint)-1;

	renderer->show();
}
void EditorHostWindow::InitializeRenderer()
{
	setCentralWidget(renderer = new Renderer());
	renderer->VERTEX_SIZE = sizeof(Neumont::Vertex);
	renderer->INDEX_SIZE = sizeof(ushort);
	renderer->POSITION_OFFSET = Neumont::Vertex::POSITION_OFFSET;
	renderer->COLOR_OFFSET = Neumont::Vertex::COLOR_OFFSET;
	renderer->NORMAL_OFFSET = Neumont::Vertex::NORMAL_OFFSET;
	renderer->UV_OFFSET = Neumont::Vertex::UV_OFFSET;

	DebugShapes::SetRenderer(renderer);
}
void EditorHostWindow::InitializeComponents()
{
	shaderID = renderer->AddShader("VertexShader.sh", "FragShader.sh");

	centralWidget()->releaseKeyboard();
}

QMenu* EditorHostWindow::BuildMenu_File()
{
	QMenu* menu = new QMenu("File");

	QAction* action = menu->addAction("Open Native Model");
	connect(action, SIGNAL(triggered()), this, SLOT(OpenFile_Native()));

	action = menu->addAction("Open FBX");
	connect(action, SIGNAL(triggered()), this, SLOT(OpenFile_FBX()));

	action = menu->addAction("Open Level");
	connect(action, SIGNAL(triggered()), this, SLOT(OpenFile_NativeLevel()));

	menu->addSeparator();

	action = menu->addAction("Save");
	connect(action, SIGNAL(triggered()), this, SLOT(SaveFile()));

	return menu;
}
QMenu* EditorHostWindow::BuildMenu_Help()
{
	QMenu* menu = new QMenu("Help");

	menu->addAction("About");

	return menu;
}
QToolBar* EditorHostWindow::BuildToolBar_AStarPathing()
{
	QToolBar* tb = new QToolBar(QString::fromUtf8("A* Pathing"), this);

	QAction* action = tb->addAction("Add Node");
	connect(action, SIGNAL(triggered()), this, SLOT(AddPathingNode()));

	tb->addSeparator();

	action = tb->addAction("Remove Unused Nodes");
	connect(action, SIGNAL(triggered()), this, SLOT(RemoveUnusedNodes()));

	tb->addSeparator();

	tb->addWidget(PathingNode_ConstantYPosition_CheckBox = new QCheckBox(tb));
	PathingNode_ConstantYPosition_CheckBox->setText("Lock Node Y-pos");

	tb->addWidget(PathingNode_TwoWayConnections_CheckBox = new QCheckBox(tb));
	PathingNode_TwoWayConnections_CheckBox->setText("Two Way Connections");

	tb->addSeparator();

	tb->addWidget(PathingNode_ID_Label = new QLabel("No node selected"));

	QWidget* empty = new QWidget();
	empty->setMinimumWidth(10);
	tb->addWidget(empty);

	tb->addWidget(new QLabel("X:"));
	tb->addWidget(PathingNode_X_Pos_LineEdit = new QLineEdit());
	PathingNode_X_Pos_LineEdit->setMaximumWidth(80);
	PathingNode_X_Pos_LineEdit->setValidator(new QDoubleValidator());

	tb->addWidget(new QLabel("Y:"));
	tb->addWidget(PathingNode_Y_Pos_LineEdit = new QLineEdit());
	PathingNode_Y_Pos_LineEdit->setMaximumWidth(80);
	PathingNode_Y_Pos_LineEdit->setValidator(new QDoubleValidator());

	tb->addWidget(new QLabel("Z:"));
	tb->addWidget(PathingNode_Z_Pos_LineEdit = new QLineEdit());
	PathingNode_Z_Pos_LineEdit->setMaximumWidth(80);
	PathingNode_Z_Pos_LineEdit->setValidator(new QDoubleValidator());

	action = tb->addAction("Set");
	connect(action, SIGNAL(triggered()), this, SLOT(SetPathingNodePosition()));

	PathingNode_X_Pos_LineEdit->setEnabled(false);
	PathingNode_Y_Pos_LineEdit->setEnabled(false);
	PathingNode_Z_Pos_LineEdit->setEnabled(false);

	tb->setContentsMargins(2, 2, 2, 2);

	return tb;
}

QString EditorHostWindow::GetCameraPositionQString()
{
	if(!renderer) return "";

	return QString("Pos: <" + 
		QString::number(camera_position.x) + "," +
		QString::number(camera_position.y) + "," +
		QString::number(camera_position.z) + ">");
}

void EditorHostWindow::OpenFile_Native()
{
	QString filename = QFileDialog::getOpenFileName(this, "Open Model File", "", "Native Models (*.bin)");
	QByteArray ba = filename.toLocal8Bit();
	char* file = ba.data();

	if(filename == "") return;

	NativeModel model;
	statusBar()->showMessage("Loading " + filename + "...", 0);
	NativeModel::LoadNativeModel(file, model);
	statusBar()->showMessage("Loaded " + filename + ".", 2000);

	for(uint i = 0; i < model.numShapeDatas; i++)
	{
		AddShapeToRenderer(model.shapeDatas[i]);
	}

}
void EditorHostWindow::OpenFile_FBX()
{
	QString filename = QFileDialog::getOpenFileName(this, "Open Model File", "", "FBX Models (*.fbx)");
	QByteArray ba = filename.toLocal8Bit();
	char* file = ba.data();

	if(filename == "") return;

	NativeModel model;
	statusBar()->showMessage("Loading " + filename + "...", 0);
	NativeModel::LoadFBX(file, model);
	statusBar()->showMessage("Loaded " + filename + ".", 2000);

	for(uint i = 0; i < model.numShapeDatas; i++)
	{
 		AddShapeToRenderer(model.shapeDatas[i]);
	}
}
void EditorHostWindow::OpenFile_NativeLevel()
{
	QString filename = QFileDialog::getOpenFileName(this, "Open Native Level File", "", "Native Levels (*.nlvl)");
	QByteArray ba = filename.toLocal8Bit();
	char* file = ba.data();

	if(filename == "") return;

	statusBar()->showMessage("Loading " + filename + "...", 0);
	level = NativeLevel_Dynamic::Load(file, renderer, shaderID);

	for(uint i = 0; i < level.renderables.size(); i++)
	{
		AddUniformsToRenderable(level.renderables[i]);
	}
	for(uint i = 0; i < level.nodes.size(); i++)
	{
		nodeRenderableIDs.push_back(DebugShapes::MakeSphere(glm::translate(level.nodes[i].position) * glm::scale(0.15f, 0.15f, 0.15f), -1.0f, true, glm::vec3(1,0,0)));
	}
	for(uint i = 0; i < level.nodes.size(); i++)
	{
		for(uint c = 0; c < level.nodes[i].connections.size(); c++)
		{
			DebugShapes::MakeVector(level.nodes[i].position, level.nodes[level.nodes[i].connections[c].connectorID].position);
		}
	}

	statusBar()->showMessage("Loaded " + filename + ".", 2000);
}
void EditorHostWindow::SaveFile()
{
	QString filename = QFileDialog::getSaveFileName(this, "Save File", "", "Native Level File (*.nlvl)");
	QByteArray ba = filename.toLocal8Bit();
	char* file = ba.data();

	statusBar()->showMessage("Saving " + filename + "...", 0);
	// save the file
	level.Save(file);
	statusBar()->showMessage("Saved " + filename + ".", 2);
}

void EditorHostWindow::AddUniformsToRenderable(Renderer::RenderableInstanceInfo* renderable)
{
	renderable->AddUniform("mvp", ut_MAT4_MVP, 0, 0);
	renderable->AddUniform("modelToWorld", ut_MAT4, &renderable->position[0][0], sizeof(glm::mat4));
	renderable->AddUniform("modelToWorld_Normal", ut_MAT3, &glm::mat3(renderable->position)[0][0], sizeof(glm::mat3));
	renderable->AddUniform("ambient", ut_VEC3, &glm::vec3(0.5f, 0.5f, 0.5f)[0], sizeof(glm::vec3));
	renderable->AddUniform("diffuse", ut_VEC3, &glm::vec3(0.5f,0.5f,0.5f)[0], sizeof(glm::vec3));
	light_position_uniformID = renderable->AddUniform("light_position", ut_VEC3, &camera_position[0], sizeof(glm::vec3));
}
uint EditorHostWindow::AddShapeToRenderer(const Neumont::ShapeData& shape)
{
	uint geoID = renderer->AddGeometry(shape.verts, shape.numVerts, shape.indices, shape.numIndices, rm_TRIANGLES);
	Renderer::RenderableInstanceInfo* instance = renderer->AddRenderableInstance_p(geoID, glm::translate(0.0f,0.0f,0.0f), shaderID);
	instance->AddUniform("mvp", ut_MAT4_MVP, 0, 0);
	instance->AddUniform("modelToWorld", ut_MAT4, &instance->position[0][0], sizeof(glm::mat4));
	instance->AddUniform("modelToWorld_Normal", ut_MAT3, &glm::mat3(instance->position)[0][0], sizeof(glm::mat3));
	instance->AddUniform("ambient", ut_VEC3, &glm::vec3(0.5f, 0.5f, 0.5f)[0], sizeof(glm::vec3));
	instance->AddUniform("diffuse", ut_VEC3, &glm::vec3(0.5f,0.5f,0.5f)[0], sizeof(glm::vec3));
	light_position_uniformID = instance->AddUniform("light_position", ut_VEC3, &camera_position[0], sizeof(glm::vec3));
	//light_position_uniformID = instance->AddUniform("light_position", ut_VEC3, &glm::vec3(0,15,0)[0], sizeof(glm::vec3));

	level.renderables.push_back(instance);
	return level.renderables.size() - 1;
}
void EditorHostWindow::Update()
{
	camera_position = renderer->camera_dbg.GetPosition();
	cameraPositionLabel->setText(GetCameraPositionQString());

	for(uint i = 0; i < level.renderables.size(); i++)
	{
		level.renderables[i]->UpdateUniform(light_position_uniformID, &camera_position[0], sizeof(glm::vec3));
	}
}

void EditorHostWindow::AddPathingNode()
{
	std::cout << "Triggered()" << std::endl;

	glm::vec3 nodePosition = renderer->camera_dbg.GetPosition();
	if(PathingNode_ConstantYPosition_CheckBox->checkState() == Qt::CheckState::Checked)
		nodePosition.y = 0.5f;

	level.AddPathingNode(nodePosition);

	nodeRenderableIDs.push_back(DebugShapes::MakeSphere(glm::translate(nodePosition) * glm::scale(0.25f, 0.25f, 0.25f), -1.0f, true, glm::vec3(1,0,0)));

	QString posString = QString("A* node added at <" + 
		QString::number(nodePosition.x) + "," +
		QString::number(nodePosition.y) + "," + 
		QString::number(nodePosition.z) + ">");

	statusBar()->showMessage(posString, 2000);
}
void EditorHostWindow::RemoveUnusedNodes()
{
	std::vector<uint> unusedIDs;

	// check each node for outgoing connections
	for(uint i = 0; i < level.nodes.size(); i++)
	{
		if(level.nodes[i].connections.size() <= 0)
		{
			unusedIDs.push_back(level.nodes[i].ID);
		}
	}

	for(uint i = 0; i < unusedIDs.size(); i++)
	{
		// check all the nodes with no outgoing connections for incoming connections
		bool hasIncomingConnection = false;
		for(uint n = 0; n < level.nodes.size(); n++)
		{
			for(uint c = 0; c < level.nodes[n].numConnections; c++)
			{
				if(level.nodes[n].connections[c].connectorID == unusedIDs[i])
				{
					hasIncomingConnection = true;
					break;
				}
			}

			if(hasIncomingConnection) break;
		}

		if(!hasIncomingConnection)
		{
			for(uint n = 0; n < level.nodes.size(); n++)
			{
				if(level.nodes[n].ID != unusedIDs[i])
				
//				DebugShapes::RemoveShape(nodeRenderableIDs[n]);				
//				nodeRenderableIDs.erase(nodeRenderableIDs.begin() + n);

//				level.RemovePathingNode(n);

				break;
			}
		}
	}
}
void EditorHostWindow::SetPathingNodePosition()
{
	if(selectedNodeIndex == (uint)-1) return;

	float x = PathingNode_X_Pos_LineEdit->text().toFloat();
	float y = PathingNode_Y_Pos_LineEdit->text().toFloat();
	float z = PathingNode_Z_Pos_LineEdit->text().toFloat();

	level.nodes[selectedNodeIndex].position = glm::vec3(x,y,z);
	DebugShapes::SetShapeTransform(nodeRenderableIDs[selectedNodeIndex], glm::translate(glm::vec3(x,y,z)) * glm::scale(0.25f, 0.25f, 0.25f));
	renderer->setFocus();
}

bool EditorHostWindow::TrySelectNode(glm::vec2 mousePos)
{
	// http://www.dreamincode.net/forums/topic/124203-ray-sphere-intersection/
	glm::vec3 rayOrigin;
	glm::vec3 rayDirection;
	renderer->RayFromMouseCoords(mousePos, &rayOrigin, &rayDirection);

	for(int i = 0; i < (int)level.nodes.size(); i++)
	{
		glm::vec3 nodeToRay = level.nodes[i].position - rayOrigin;
		float distanceFromRay = glm::length(nodeToRay);
		float differenceFromRay = glm::dot(nodeToRay, rayDirection);
		float d = (0.15f * 0.15f) - ((distanceFromRay * distanceFromRay) - (differenceFromRay * differenceFromRay));

		if(differenceFromRay - sqrtf(d) >= 0)
		{
			selectedNodeIndex = i;
			DebugShapes::SetShapeColor(nodeRenderableIDs[i], glm::vec3(0,1,0));

			PathingNode_ID_Label->setText("Selected Node: " + QString::number(i));

			PathingNode_X_Pos_LineEdit->setText(QString::number(level.nodes[i].position.x));
			PathingNode_Y_Pos_LineEdit->setText(QString::number(level.nodes[i].position.y));
			PathingNode_Z_Pos_LineEdit->setText(QString::number(level.nodes[i].position.z));

			PathingNode_X_Pos_LineEdit->setEnabled(true);
			PathingNode_Y_Pos_LineEdit->setEnabled(true);
			PathingNode_Z_Pos_LineEdit->setEnabled(true);

			return true;
		}
	}

	return false;
}
void EditorHostWindow::DeselectNode()
{
	DebugShapes::SetShapeColor(nodeRenderableIDs[selectedNodeIndex], glm::vec3(1,0,0));
	selectedNodeIndex = (uint)-1;

	PathingNode_ID_Label->setText("No node selected");

	PathingNode_X_Pos_LineEdit->setText("");
	PathingNode_Y_Pos_LineEdit->setText("");
	PathingNode_Z_Pos_LineEdit->setText("");

	PathingNode_X_Pos_LineEdit->setEnabled(false);
	PathingNode_Y_Pos_LineEdit->setEnabled(false);
	PathingNode_Z_Pos_LineEdit->setEnabled(false);
}
bool EditorHostWindow::TryConnectNode(glm::vec2 mousePos)
{
	glm::vec3 rayOrigin;
	glm::vec3 rayDirection;
	renderer->RayFromMouseCoords(mousePos, &rayOrigin, &rayDirection);

	for(int i = 0; i < (int)level.nodes.size(); i++)
	{
		glm::vec3 nodeToRay = level.nodes[i].position - rayOrigin;
		float distanceFromRay = glm::length(nodeToRay);
		float differenceFromRay = glm::dot(nodeToRay, rayDirection);
		float d = (0.15f * 0.15f) - ((distanceFromRay * distanceFromRay) - (differenceFromRay * differenceFromRay));

		if(d >= 0)
		{
			// if the node pressed is the selected node, break. (don't allow nodes to connect to themselves)
			if(i == selectedNodeIndex) break;
			
			level.nodes[selectedNodeIndex].AddConnection(level.nodes[i]);
			DebugShapes::MakeVector(level.nodes[selectedNodeIndex].position, level.nodes[i].position);

			if(PathingNode_TwoWayConnections_CheckBox->checkState() == Qt::CheckState::Checked)
			{
				level.nodes[i].AddConnection(level.nodes[selectedNodeIndex]);
				DebugShapes::MakeVector(level.nodes[i].position, level.nodes[selectedNodeIndex].position);
			}

			return true;
		}
	}

	return false;
}

void EditorHostWindow::ShowNodeTools()
{

}
void EditorHostWindow::HideNodeTools()
{

}

void EditorHostWindow::Renderer_MousePressed(glm::vec2 mousePos)
{
	renderer->setFocus();

	if(selectedNodeIndex == (uint)-1)
	{
		TrySelectNode(mousePos);
	}
	else
	{
		if(!TryConnectNode(mousePos))
			DeselectNode();
	}
}
void EditorHostWindow::Renderer_MouseMoved(glm::vec2 mousePos)
{

}

void EditorHostWindow::keyPressEvent(QKeyEvent *e)
{
	if(e->key() == Qt::Key::Key_Escape) exit(0);

	renderer->keyPressEvent(e);
}

#include "EditorHostWindow_moc.cpp"