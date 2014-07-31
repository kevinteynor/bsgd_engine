#include <Qt\qapplication.h>
#include "ParentWindow.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	ParentWindow window;
	window.Initialize();

	return app.exec();
}