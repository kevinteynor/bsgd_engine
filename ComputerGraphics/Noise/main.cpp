#include <Qt\qapplication.h>
#include "NoiseDisplay.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	NoiseDisplay display;
	display.Initialize();
	display.show();

	return app.exec();
}