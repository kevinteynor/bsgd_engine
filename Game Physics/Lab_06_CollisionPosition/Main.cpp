#include <Qt\qapplication.h>
#include "CollisionPositionDemo.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	CollisionPositionDemo window;
	window.Initialize();

	app.exec();
}