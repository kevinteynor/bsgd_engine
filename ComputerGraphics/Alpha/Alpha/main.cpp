#include <Qt\qapplication.h>
#include "AlphaDisplay.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	AlphaDisplay display;
	display.Initialize();
	display.show();

	app.exec();
}