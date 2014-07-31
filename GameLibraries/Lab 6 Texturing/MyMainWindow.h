#ifndef MY_MAIN_WINDOW
#define MY_MAIN_WINDOW
#include <QtGui\qwidget.h>

class MyGLWidget;
namespace Neumont { class Slider; }

class MyMainWindow : public QWidget
{
	Q_OBJECT

private:

#pragma region Slider Objects
	// Light Position
	Neumont::Slider* slider_lightPos_X;
	Neumont::Slider* slider_lightPos_Y;
	Neumont::Slider* slider_lightPos_Z;

	// Light Color
	Neumont::Slider* slider_lightColor_R;
	Neumont::Slider* slider_lightColor_G;
	Neumont::Slider* slider_lightColor_B;

	// Ambient Light
	Neumont::Slider* slider_ambient_X;
	Neumont::Slider* slider_ambient_Y;
	Neumont::Slider* slider_ambient_Z;

	// Plane Ambient
	Neumont::Slider* slider_planeAmbient_X;
	Neumont::Slider* slider_planeAmbient_Y;
	Neumont::Slider* slider_planeAmbient_Z;

	// Plane Diffuse
	Neumont::Slider* slider_planeDiffuse_X;
	Neumont::Slider* slider_planeDiffuse_Y;
	Neumont::Slider* slider_planeDiffuse_Z;

	// Plane Specular
	Neumont::Slider* slider_planeSpecular_X;
	Neumont::Slider* slider_planeSpecular_Y;
	Neumont::Slider* slider_planeSpecular_Z;

	// Torus Ambient
	Neumont::Slider* slider_torusAmbient_X;
	Neumont::Slider* slider_torusAmbient_Y;
	Neumont::Slider* slider_torusAmbient_Z;

	// Torus Diffuse
	Neumont::Slider* slider_torusDiffuse_X;
	Neumont::Slider* slider_torusDiffuse_Y;
	Neumont::Slider* slider_torusDiffuse_Z;

	// Torus Specular
	Neumont::Slider* slider_torusSpecular_X;
	Neumont::Slider* slider_torusSpecular_Y;
	Neumont::Slider* slider_torusSpecular_Z;

	// Teapot Ambient
	Neumont::Slider* slider_teapotAmbient_X;
	Neumont::Slider* slider_teapotAmbient_Y;
	Neumont::Slider* slider_teapotAmbient_Z;

	// Teapot Diffuse
	Neumont::Slider* slider_teapotDiffuse_X;
	Neumont::Slider* slider_teapotDiffuse_Y;
	Neumont::Slider* slider_teapotDiffuse_Z;

	// Teapot Specular
	Neumont::Slider* slider_teapotSpecular_X;
	Neumont::Slider* slider_teapotSpecular_Y;
	Neumont::Slider* slider_teapotSpecular_Z;

#pragma endregion 

	MyGLWidget* renderWindow;
	void SetUpGUI();

	private slots:
		void GUIValueChanged();
		void CheckboxTicked();
		void discardCbxTicked();

public:
	MyMainWindow(void);
	~MyMainWindow(void);
};

#endif // MY_MAIN_WINDOW