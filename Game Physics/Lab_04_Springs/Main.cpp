#include <Qt\qapplication.h>
#include "SpringDemo.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	SpringDemo window;
	window.Initialize();

	app.exec();
}