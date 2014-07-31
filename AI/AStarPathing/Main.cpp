#include <Qt\qapplication.h>
#include "PathingDisplayWindow.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	PathingDisplayWindow window;
	window.Initialize();
	window.show();

	app.exec();
}