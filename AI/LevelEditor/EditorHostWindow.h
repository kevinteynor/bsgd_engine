#ifndef EDITOR_HOST_WINDOW_H
#define EDITOR_HOST_WINDOW_H

#include <AStarPath.h>

#include <glm\glm.hpp>
#include <NativeLevel.h>
#include <NativeModel.h>
#include <Qt\qmainwindow.h>
#include <Qt\qstring.h>
#include <Qt\qwidget.h>
#include <Renderer.h>
#include <ShapeData.h>
#include <vector>
#include <DebugShapes.h>

class DebugCamera;
class QCheckBox;
class QLabel;
class QLineEdit;
class QMenu;
class QMenuBar;
class QToolBar;
class QHBoxLayout;
class QVBoxLayout;

class EditorHostWindow : public QMainWindow
{
	Q_OBJECT

private:
	DebugCamera* debugCamera;
	Renderer* renderer;
	uint shaderID;
	glm::vec3 camera_position;

	uint selectedNodeIndex;
	std::vector<uint> nodeRenderableIDs;
	NativeLevel_Dynamic level;

	void AddUniformsToRenderable(Renderer::RenderableInstanceInfo* renderable);
	uint AddShapeToRenderer(const Neumont::ShapeData& shape);
	bool TrySelectNode(glm::vec2);
	void DeselectNode();
	bool TryConnectNode(glm::vec2);

	void ShowNodeTools();
	void HideNodeTools();

#pragma region UI_SETUP_CODE
	QMenu* BuildMenu_File();
	QMenu* BuildMenu_Help();

	QToolBar* BuildToolBar_AStarPathing();
	QCheckBox* PathingNode_ConstantYPosition_CheckBox;
	QCheckBox* PathingNode_TwoWayConnections_CheckBox;

	QLabel* PathingNode_ID_Label;
	QLineEdit* PathingNode_X_Pos_LineEdit;
	QLineEdit* PathingNode_Y_Pos_LineEdit;
	QLineEdit* PathingNode_Z_Pos_LineEdit;

	QLabel* cameraPositionLabel;
	QString GetCameraPositionQString();
#pragma endregion

	void InitializeRenderer();

#pragma region SHADER_UNIFORMS_CODE	
	uint light_position_uniformID;
	Renderer::RenderableInstanceInfo* instanceInfo;

#pragma endregion

public:
	EditorHostWindow(){}
	~EditorHostWindow(){}
	
	void Initialize();

protected:
	void keyPressEvent(QKeyEvent *e);

public slots:
	void Update();
	void InitializeComponents();
	void OpenFile_Native();
	void OpenFile_FBX();
	void OpenFile_NativeLevel();
	void SaveFile();

	void AddPathingNode();
	void RemoveUnusedNodes();
	void SetPathingNodePosition();

	void Renderer_MousePressed(glm::vec2);
	void Renderer_MouseMoved(glm::vec2);
};

#endif // EDITOR_HOST_WINDOW_H