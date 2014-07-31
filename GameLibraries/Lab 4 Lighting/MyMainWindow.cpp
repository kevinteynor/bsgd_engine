#include "MyMainWindow.h"
#include <Qt\qdebug.h>
#include <QtGui\qvboxlayout>
#include <QtGui\qhboxlayout>
#include <QtGui\qcheckbox.h>
#include <QtGui\qlabel.h>
#include "MyGLWindow.h"
#include "Slider.h"

MyMainWindow::MyMainWindow()
{
	SetUpGUI();
}
MyMainWindow::~MyMainWindow()
{

}

void MyMainWindow::SetUpGUI()
{
	setMinimumSize(500,500);
	QVBoxLayout* mainLayout;
	setLayout(mainLayout = new QVBoxLayout);

	// initialize row layouts
	QHBoxLayout* rowLayout_top;
	QHBoxLayout* rowLayout_middle;
	QHBoxLayout* rowLayout_bottom;

	mainLayout->addLayout(rowLayout_top = new QHBoxLayout);
	mainLayout->addLayout(rowLayout_middle = new QHBoxLayout);
	mainLayout->addLayout(rowLayout_bottom = new QHBoxLayout);

#pragma region Column 1
	// Light Position
	rowLayout_top->addWidget(new QLabel("Light Position: <"));
	rowLayout_top->addWidget(slider_lightPos_X = new Neumont::Slider);	
	rowLayout_top->addWidget(new QLabel(","));

	rowLayout_top->addWidget(slider_lightPos_Y = new Neumont::Slider);
	rowLayout_top->addWidget(new QLabel(","));

	rowLayout_top->addWidget(slider_lightPos_Z = new Neumont::Slider);
	rowLayout_top->addWidget(new QLabel(">"));

	// Light Color
	rowLayout_middle->addWidget(new QLabel("Light Color: <"));
	rowLayout_middle->addWidget(slider_lightColor_R = new Neumont::Slider(0, 1));
	rowLayout_middle->addWidget(new QLabel(","));

	rowLayout_middle->addWidget(slider_lightColor_G = new Neumont::Slider(0, 1));
	rowLayout_middle->addWidget(new QLabel(","));

	rowLayout_middle->addWidget(slider_lightColor_B = new Neumont::Slider(0, 1));
	rowLayout_middle->addWidget(new QLabel(">"));

	// Ambient Light sliders
	rowLayout_bottom->addWidget(new QLabel("Ambient Light: <"));
	rowLayout_bottom->addWidget(slider_ambient_X = new Neumont::Slider(0, 1));	
	rowLayout_bottom->addWidget(new QLabel(","));

	rowLayout_bottom->addWidget(slider_ambient_Y = new Neumont::Slider(0, 1));	
	rowLayout_bottom->addWidget(new QLabel(","));

	rowLayout_bottom->addWidget(slider_ambient_Z = new Neumont::Slider(0, 1));
	rowLayout_bottom->addWidget(new QLabel(">"));
#pragma endregion
#pragma region Column 2
	// Plane Ambient
	rowLayout_top->addWidget(new QLabel("Plane Ambient: <"));
	rowLayout_top->addWidget(slider_planeAmbient_X = new Neumont::Slider(0, 1));	
	rowLayout_top->addWidget(new QLabel(","));

	rowLayout_top->addWidget(slider_planeAmbient_Y = new Neumont::Slider(0, 1));	
	rowLayout_top->addWidget(new QLabel(","));

	rowLayout_top->addWidget(slider_planeAmbient_Z = new Neumont::Slider(0, 1));	
	rowLayout_top->addWidget(new QLabel(">"));

	// Plane Diffuse
	rowLayout_middle->addWidget(new QLabel("Plane Diffuse: <"));
	rowLayout_middle->addWidget(slider_planeDiffuse_X = new Neumont::Slider(0, 1));	
	rowLayout_middle->addWidget(new QLabel(","));

	rowLayout_middle->addWidget(slider_planeDiffuse_Y = new Neumont::Slider(0, 1));	
	rowLayout_middle->addWidget(new QLabel(","));

	rowLayout_middle->addWidget(slider_planeDiffuse_Z = new Neumont::Slider(0, 1));	
	rowLayout_middle->addWidget(new QLabel(">"));

	// Plane Specular
	rowLayout_bottom->addWidget(new QLabel("Plane Specular: <"));
	rowLayout_bottom->addWidget(slider_planeSpecular_X = new Neumont::Slider(0, 1));	
	rowLayout_bottom->addWidget(new QLabel(","));

	rowLayout_bottom->addWidget(slider_planeSpecular_Y = new Neumont::Slider(0, 1));	
	rowLayout_bottom->addWidget(new QLabel(","));

	rowLayout_bottom->addWidget(slider_planeSpecular_Z = new Neumont::Slider(0, 1));	
	rowLayout_bottom->addWidget(new QLabel(">"));

#pragma endregion
#pragma region Column 3
	// Torus Ambient
	rowLayout_top->addWidget(new QLabel("Torus Ambient: <"));
	rowLayout_top->addWidget(slider_torusAmbient_X = new Neumont::Slider(0, 1));	
	rowLayout_top->addWidget(new QLabel(","));

	rowLayout_top->addWidget(slider_torusAmbient_Y = new Neumont::Slider(0, 1));	
	rowLayout_top->addWidget(new QLabel(","));

	rowLayout_top->addWidget(slider_torusAmbient_Z = new Neumont::Slider(0, 1));	
	rowLayout_top->addWidget(new QLabel(">"));

	// Torus Diffuse
	rowLayout_middle->addWidget(new QLabel("Torus Diffuse: <"));
	rowLayout_middle->addWidget(slider_torusDiffuse_X = new Neumont::Slider(0, 1));	
	rowLayout_middle->addWidget(new QLabel(","));

	rowLayout_middle->addWidget(slider_torusDiffuse_Y = new Neumont::Slider(0, 1));	
	rowLayout_middle->addWidget(new QLabel(","));

	rowLayout_middle->addWidget(slider_torusDiffuse_Z = new Neumont::Slider(0, 1));	
	rowLayout_middle->addWidget(new QLabel(">"));

	// Torus Specular
	rowLayout_bottom->addWidget(new QLabel("Torus Specular: <"));
	rowLayout_bottom->addWidget(slider_torusSpecular_X = new Neumont::Slider(0, 1));	
	rowLayout_bottom->addWidget(new QLabel(","));

	rowLayout_bottom->addWidget(slider_torusSpecular_Y = new Neumont::Slider(0, 1));	
	rowLayout_bottom->addWidget(new QLabel(","));

	rowLayout_bottom->addWidget(slider_torusSpecular_Z = new Neumont::Slider(0, 1));	
	rowLayout_bottom->addWidget(new QLabel(">"));

#pragma endregion
#pragma region Column 4
	// Teapot Ambient
	rowLayout_top->addWidget(new QLabel("Teapot Ambient: <"));
	rowLayout_top->addWidget(slider_teapotAmbient_X = new Neumont::Slider(0, 1));	
	rowLayout_top->addWidget(new QLabel(","));

	rowLayout_top->addWidget(slider_teapotAmbient_Y = new Neumont::Slider(0, 1));	
	rowLayout_top->addWidget(new QLabel(","));

	rowLayout_top->addWidget(slider_teapotAmbient_Z = new Neumont::Slider(0, 1));	
	rowLayout_top->addWidget(new QLabel(">"));

	// Teapot Diffuse
	rowLayout_middle->addWidget(new QLabel("Teapot Diffuse: <"));
	rowLayout_middle->addWidget(slider_teapotDiffuse_X = new Neumont::Slider(0, 1));	
	rowLayout_middle->addWidget(new QLabel(","));

	rowLayout_middle->addWidget(slider_teapotDiffuse_Y = new Neumont::Slider(0, 1));	
	rowLayout_middle->addWidget(new QLabel(","));

	rowLayout_middle->addWidget(slider_teapotDiffuse_Z = new Neumont::Slider(0, 1));	
	rowLayout_middle->addWidget(new QLabel(">"));

	// Teapot Specular
	rowLayout_bottom->addWidget(new QLabel("Teapot Specular: <"));
	rowLayout_bottom->addWidget(slider_teapotSpecular_X = new Neumont::Slider(0, 1));	
	rowLayout_bottom->addWidget(new QLabel(","));

	rowLayout_bottom->addWidget(slider_teapotSpecular_Y = new Neumont::Slider(0, 1));	
	rowLayout_bottom->addWidget(new QLabel(","));

	rowLayout_bottom->addWidget(slider_teapotSpecular_Z = new Neumont::Slider(0, 1));	
	rowLayout_bottom->addWidget(new QLabel(">"));

#pragma endregion

#pragma region Initialize Values
	slider_ambient_X->setValue(0.1f);
	slider_ambient_Y->setValue(0.1f);
	slider_ambient_Z->setValue(0.1f);

	slider_lightColor_R->setValue(1.0f);
	slider_lightColor_G->setValue(1.0f);
	slider_lightColor_B->setValue(1.0f);
	
	slider_lightPos_Y->setValue(2.0f);

	slider_planeAmbient_X->setValue(0.5f);
	slider_planeAmbient_Y->setValue(0.5f);
	slider_planeAmbient_Z->setValue(0.5f);

	slider_planeDiffuse_X->setValue(0.5f);
	slider_planeDiffuse_Y->setValue(0.5f);
	slider_planeDiffuse_Z->setValue(0.5f);

	slider_planeDiffuse_X->setValue(0.5f);
	slider_planeDiffuse_Y->setValue(0.5f);
	slider_planeDiffuse_Z->setValue(0.5f);

	slider_planeSpecular_X->setValue(0.5f);
	slider_planeSpecular_Y->setValue(0.5f);
	slider_planeSpecular_Z->setValue(0.5f);

	slider_torusAmbient_X->setValue(0.5f);
	slider_torusAmbient_Y->setValue(0.5f);
	slider_torusAmbient_Z->setValue(0.5f);

	slider_torusDiffuse_X->setValue(0.5f);
	slider_torusDiffuse_Y->setValue(0.5f);
	slider_torusDiffuse_Z->setValue(0.5f);

	slider_torusSpecular_X->setValue(0.5f);
	slider_torusSpecular_Y->setValue(0.5f);
	slider_torusSpecular_Z->setValue(0.5f);

	slider_teapotAmbient_X->setValue(0.5f);
	slider_teapotAmbient_Y->setValue(0.5f);
	slider_teapotAmbient_Z->setValue(0.5f);

	slider_teapotDiffuse_X->setValue(0.5f);
	slider_teapotDiffuse_Y->setValue(0.5f);
	slider_teapotDiffuse_Z->setValue(0.5f);

	slider_teapotSpecular_X->setValue(0.5f);
	slider_teapotSpecular_Y->setValue(0.5f);
	slider_teapotSpecular_Z->setValue(0.5f);
#pragma endregion

#pragma region Connections
	// column 1
	connect(slider_lightPos_X, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));
	connect(slider_lightPos_Y, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));
	connect(slider_lightPos_Z, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));

	connect(slider_lightColor_R, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));
	connect(slider_lightColor_G, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));
	connect(slider_lightColor_B, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));

	connect(slider_ambient_X, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));
	connect(slider_ambient_Y, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));
	connect(slider_ambient_Z, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));

	// column 2
	connect(slider_planeAmbient_X, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));
	connect(slider_planeAmbient_Y, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));
	connect(slider_planeAmbient_Z, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));

	connect(slider_planeDiffuse_X, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));
	connect(slider_planeDiffuse_Y, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));
	connect(slider_planeDiffuse_Z, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));

	connect(slider_planeSpecular_X, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));
	connect(slider_planeSpecular_Y, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));
	connect(slider_planeSpecular_Z, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));

	// column 3
	connect(slider_torusAmbient_X, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));
	connect(slider_torusAmbient_Y, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));
	connect(slider_torusAmbient_Z, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));

	connect(slider_torusDiffuse_X, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));
	connect(slider_torusDiffuse_Y, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));
	connect(slider_torusDiffuse_Z, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));

	connect(slider_torusSpecular_X, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));
	connect(slider_torusSpecular_Y, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));
	connect(slider_torusSpecular_Z, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));

	// column 4
	connect(slider_teapotAmbient_X, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));
	connect(slider_teapotAmbient_Y, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));
	connect(slider_teapotAmbient_Z, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));

	connect(slider_teapotDiffuse_X, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));
	connect(slider_teapotDiffuse_Y, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));
	connect(slider_teapotDiffuse_Z, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));

	connect(slider_teapotSpecular_X, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));
	connect(slider_teapotSpecular_Y, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));
	connect(slider_teapotSpecular_Z, SIGNAL(valueChanged(float)), this, SLOT(GUIValueChanged()));

#pragma endregion
	
	QCheckBox* runCalculationsInFragShaderCbx;
	mainLayout->addWidget(runCalculationsInFragShaderCbx = new QCheckBox("Run calculations in fragment shader"));
	connect(runCalculationsInFragShaderCbx, SIGNAL(toggled(bool)), this, SLOT(CheckboxTicked()));

	mainLayout->addWidget(renderWindow = new MyGLWidget);
}

void MyMainWindow::GUIValueChanged()
{

	renderWindow->lightPosition = glm::vec3(slider_lightPos_X->value(), slider_lightPos_Y->value(), slider_lightPos_Z->value());
	renderWindow->lightColor = glm::vec3(slider_lightColor_R->value(), slider_lightColor_G->value(), slider_lightColor_B->value());
	renderWindow->ambientColor = glm::vec3(slider_ambient_X->value(), slider_ambient_Y->value(), slider_ambient_Z->value());

	renderWindow->planeAmbient = glm::vec3(slider_planeAmbient_X->value(), slider_planeAmbient_Y->value(), slider_planeAmbient_Z->value());
	renderWindow->planeDiffuse = glm::vec3(slider_planeDiffuse_X->value(), slider_planeDiffuse_Y->value(), slider_planeDiffuse_Z->value());
	renderWindow->planeSpecular = glm::vec3(slider_planeSpecular_X->value(), slider_planeSpecular_Y->value(), slider_planeSpecular_Z->value());

	renderWindow->torusAmbient = glm::vec3(slider_torusAmbient_X->value(), slider_torusAmbient_Y->value(), slider_torusAmbient_Z->value());
	renderWindow->torusDiffuse = glm::vec3(slider_torusDiffuse_X->value(), slider_torusDiffuse_Y->value(), slider_torusDiffuse_Z->value());
	renderWindow->torusSpecular = glm::vec3(slider_torusSpecular_X->value(), slider_torusSpecular_Y->value(), slider_torusSpecular_Z->value());

	renderWindow->teapotAmbient = glm::vec3(slider_teapotAmbient_X->value(), slider_teapotAmbient_Y->value(), slider_teapotAmbient_Z->value());
	renderWindow->teapotDiffuse = glm::vec3(slider_teapotDiffuse_X->value(), slider_teapotDiffuse_Y->value(), slider_teapotDiffuse_Z->value());
	renderWindow->teapotSpecular = glm::vec3(slider_teapotSpecular_X->value(), slider_teapotSpecular_Y->value(), slider_teapotSpecular_Z->value());

	renderWindow->updateShaders();
}
void MyMainWindow::CheckboxTicked()
{
	renderWindow->RunFragShader = !renderWindow->RunFragShader;
	renderWindow->updateShaders();
}