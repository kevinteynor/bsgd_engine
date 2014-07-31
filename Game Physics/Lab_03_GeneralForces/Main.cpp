#include <Qt\qapplication.h>
#include "GeneralForceDemo.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	GeneralForceDemo window;
	window.Initialize();

	return app.exec();
}