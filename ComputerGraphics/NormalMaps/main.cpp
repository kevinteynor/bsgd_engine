#include <Qt\qapplication.h>
#include "NormalDisplay.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	NormalDisplay display;
	display.Initialize();
	display.show();

	app.exec();
}