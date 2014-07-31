#include <Qt\qapplication.h>
#include "SimpleCollisionDemo.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	SimpleCollisionDemo window;
	window.Initialize();

	app.exec();
}