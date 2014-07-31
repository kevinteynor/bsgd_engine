#include <Qt\qapplication.h>
#include "CubeMapDisplay.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	CubeMapDisplay display;
	display.Initialize();
	display.show();

	app.exec();
}