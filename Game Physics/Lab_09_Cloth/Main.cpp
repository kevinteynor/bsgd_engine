#include <Qt\qapplication.h>
#include "ClothDemo.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	ClothDemo window;
	window.Initialize();

	app.exec();
}