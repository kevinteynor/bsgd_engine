#include <Qt\qapplication.h>
#include "ShadowDisplay.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	ShadowDisplay display;
	display.Initialize();
	display.show();

	app.exec();
}