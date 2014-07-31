#include <Qt\qapplication.h>
#include "MyGLWindow.h"
//#include <Qt\qwidget.h>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	MyGLWidget widget;
	widget.show();
	return app.exec();
}