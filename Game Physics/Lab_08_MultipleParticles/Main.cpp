#include <Qt\qapplication.h>
#include "MultipleParticlesDemo.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	MultipleParticlesDemo window;
	window.Initialize();

	app.exec();
}