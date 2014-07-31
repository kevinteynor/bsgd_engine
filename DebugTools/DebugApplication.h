#ifndef DEBUG_APPLICATION
#define DEBUG_APPLICATION

#include <glm\glm.hpp>
#include <Qt\qcheckbox.h>
#include <Qt\qdockwidget.h>
#include <Qt\qboxlayout.h>
#include <Qt\qlabel.h>
#include <Qt\qmainwindow.h>
#include <Qt\qpushbutton.h>
#include <Qt\qslider.h>
#include <Qt\qvector.h>
#include "DebugShapes.h"
#include "ExportHeader.h"
#include "FastDelegate.h"

namespace Neumont { class Slider; }
class Renderer;

namespace DebugTools
{
	struct FloatSlider;
	struct FloatWatchInfo;
	struct FloatWatchSliderInfo;
	struct BoolWatcherInfo;
	struct Vec2WatchInfo;
	struct Vec3WatchInfo;
	struct ButtonWatchInfo;

	class DEBUG_SHARED DebugApplication : public QMainWindow
	{
		Q_OBJECT

	public:
		QDockWidget* debugWidget;
		Renderer* renderer;

		DebugApplication(){}
		~DebugApplication(){}
		virtual void Initialize();

	public slots:
		virtual void MousePressed(glm::vec2);
		virtual void MouseMoved(glm::vec2);
		virtual void RendererInitialized();
		virtual void Update();
		void DockWidgetOrientationChanged(Qt::DockWidgetArea area);

	};

	class DEBUG_SHARED DebugWidget : public QObject
	{
		Q_OBJECT

	private:
		DebugWidget(void);
		~DebugWidget(void);
		DebugWidget(const DebugWidget&);
		DebugWidget& operator=(const DebugWidget&);

		static QDockWidget* activeDockWidget;
		static QWidget* activeDebugWidget;
		static QBoxLayout* mainLayout;

		static QVector<FloatWatchInfo> floatWatchInfos;
		static QVector<FloatWatchSliderInfo> floatWatchSliderInfos;
		static QVector<BoolWatcherInfo> boolWatchInfos;
		static QVector<Vec2WatchInfo> vec2WatchInfos;
		static QVector<Vec3WatchInfo> vec3WatchInfos;
		static QVector<ButtonWatchInfo*> buttonWatchInfos;

	public:
		static Renderer* ActiveRenderer;
		static void SetActiveDebugDockWidget(QDockWidget* widget);
		static void UpdateWatchInfos();
		static void ToggleLayoutOrientation(int orientation = 0);

		static FloatWatchInfo* WatchFloat(const char* text, const float* value);
		static FloatWatchSliderInfo* WatchFloatSlider(const char* text, float* value, float min = -10, float max = 10);
		static BoolWatcherInfo* WatchBool(const char* text, bool* value);
		static Vec2WatchInfo* WatchVec2(const char* text, float* x, float* y);
		static Vec3WatchInfo* WatchVec3(const char* text, float* x, float* y, float* z);
		static ButtonWatchInfo** WatchButton(const char* text, Delegate function);

	};
	
	struct FloatWatchInfo
	{
	public:
		const float* theFloat;
		QLabel* theLabel;

		~FloatWatchInfo(){ theFloat = 0; theLabel = 0; }

	};
	struct FloatWatchSliderInfo
	{
		float* theFloat;
		Neumont::Slider* theSlider;
		float min, max;

		~FloatWatchSliderInfo() { theFloat = 0; theSlider = 0; }
	};
	struct BoolWatcherInfo
	{
		bool* theBool;
		QCheckBox* theCheckbox;

		~BoolWatcherInfo(){ theBool = 0; theCheckbox = 0; }
	};
	struct Vec2WatchInfo
	{
		float* x;
		float* y;
		QLabel* theLabel;

		~Vec2WatchInfo(){ x = 0; y = 0; theLabel = 0; }
	};
	struct Vec3WatchInfo
	{
		float* x;
		float* y;
		float* z;
		QLabel* theLabel;

		~Vec3WatchInfo(){ x = 0; y = 0; z = 0; theLabel = 0; }
	};
	struct ButtonWatchInfo : QObject
	{
		Q_OBJECT

	public:
		Delegate buttonFunction;
		QPushButton* theButton;

		void ConnectButton();
		ButtonWatchInfo(){}
		ButtonWatchInfo(const ButtonWatchInfo& ref){}
		~ButtonWatchInfo() { theButton = 0; }

	public slots:
		void functioncall();
	};

}
#endif // DEBUG_APPLICATION