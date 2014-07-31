#include <Qt\qapplication.h>
#include "MyGLWindow.h"
#include "MyMainWindow.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	//MyGLWidget widget;
	MyMainWindow widget;
	widget.show();
	return app.exec();
}