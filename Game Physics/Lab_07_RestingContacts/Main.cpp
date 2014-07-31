#include <Qt\qapplication.h>
#include "RestingContactsDemo.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	RestingContactsDemo window;
	window.Initialize();

	app.exec();
}