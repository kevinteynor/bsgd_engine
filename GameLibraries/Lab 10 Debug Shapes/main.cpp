#include <Qt\qapplication.h>
#include "ParentWindow.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	ParentWindow window;
	window.InitializeRenderer();

	return app.exec();
}